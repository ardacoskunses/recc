/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include "linker.h"

void buffered_token_output(struct unsigned_char_list *, struct asm_lexer_token *);
void free_symbol_map(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *);
unsigned int set_post_linking_offsets(struct linker_object * linker_object);
unsigned int get_instruction_size(struct asm_instruction *);


unsigned int get_instruction_size(struct asm_instruction * instruction){
	enum asm_token_type type = instruction->op_token->type;

	if ((type == A_BLT || type == A_BEQ) && instruction->identifier_token){
		return 4;
	}

	if (type == A_LL && instruction->identifier_token){
		return 9;
	}

	return 1; /* 1 word */
}

unsigned int set_post_linking_offsets(struct linker_object * linker_object){
	unsigned int i;
	unsigned int num_instructions = struct_asm_instruction_ptr_list_size(&linker_object->instructions);
	unsigned int current_offset = 0;
	for(i = 0; i < num_instructions; i++){
		struct asm_instruction * instruction = struct_asm_instruction_ptr_list_get(&linker_object->instructions, i);
		instruction->post_linking_offset = current_offset;
		current_offset += get_instruction_size(instruction);
	}
	return current_offset;
}

void set_symbol_instruction_index(struct linker_object * linker_object, struct asm_lexer_token * token, unsigned int index){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte);
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->symbols, identifier_str);
	if(existing_symbol){
		existing_symbol->instruction_index = index;
	}else{
		printf("Undeclared dentifier %s on line %d in file %s\n", identifier_str, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
		assert(0 && "Trying to set offset of unknown symbol.");
	}
	free(identifier_str);
}

void verify_symbol_declaration(struct linker_object * linker_object, struct asm_lexer_token * token){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte);
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->symbols, identifier_str);
	if(!existing_symbol){
		printf("Undeclared dentifier %s on line %d in file %s\n", identifier_str, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
		assert(0 && "Found symbol without forward declaration.");
	}
	free(identifier_str);
}

void add_linker_symbol(struct linker_object * linker_object, struct asm_lexer_token * token, unsigned int is_external, unsigned int is_required, unsigned int is_implemented){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte);
	struct linker_symbol * new_symbol;
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->symbols, identifier_str);
	if(existing_symbol){
		assert(existing_symbol->is_external == is_external && "Symbol type can't change between internal and external.");
		existing_symbol->is_implemented = existing_symbol->is_implemented ? existing_symbol->is_implemented : is_implemented;
		existing_symbol->is_required = existing_symbol->is_required ? existing_symbol->is_required : is_required;
		existing_symbol->is_external = is_external;
		free(identifier_str);
		return;
	}
	new_symbol = malloc(sizeof(struct linker_symbol));
	new_symbol->is_implemented = is_implemented;
	new_symbol->is_required = is_required;
	new_symbol->is_external = is_external;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(&linker_object->symbols, identifier_str, new_symbol);
}

struct linker_object * process_assembly(struct asm_lexer_state * asm_lexer_state){
	struct linker_object * linker_object = malloc(sizeof(struct linker_object));
	unsigned int num_tokens = struct_asm_lexer_token_ptr_list_size(&asm_lexer_state->tokens);
	struct asm_lexer_token ** tokens =  struct_asm_lexer_token_ptr_list_data(&asm_lexer_state->tokens);
	unsigned int i = 0;
	linker_object->is_relocatable = 0; /* Assume false */
	linker_object->current_line = 1;
	linker_object->asm_lexer_state = asm_lexer_state;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_create(&linker_object->symbols, unsigned_strcmp);
	struct_asm_instruction_ptr_list_create(&linker_object->instructions);
	while(i < num_tokens){
		if(tokens[i]->type == A_ADD || tokens[i]->type == A_SUB || tokens[i]->type == A_MUL || tokens[i]->type == A_AND || tokens[i]->type == A_OR){
			struct asm_instruction * new_instruction = malloc(sizeof(struct asm_instruction));
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_REGISTER){
							new_instruction->ry_token = tokens[i];
							i++;
							if(tokens[i]->type == A_SPACE){
								i++;
								if(tokens[i]->type == A_REGISTER){
									new_instruction->rz_token = tokens[i];
									i++;
									struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
								}else{ assert(0 && "Expected register."); }
							}else{ assert(0 && "Expected space."); }
						}else{ assert(0 && "Expected register."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_LOA || tokens[i]->type == A_STO || tokens[i]->type == A_NOT || tokens[i]->type == A_SHR || tokens[i]->type == A_SHL){
			struct asm_instruction * new_instruction = malloc(sizeof(struct asm_instruction));
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_REGISTER){
							new_instruction->ry_token = tokens[i];
							i++;
							struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
						}else{ assert(0 && "Expected register."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_LL){
			struct asm_instruction * new_instruction = malloc(sizeof(struct asm_instruction));
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_CONSTANT_HEX){
							new_instruction->number_token_is_negative = 0;
							new_instruction->number_token = tokens[i];
							new_instruction->identifier_token = 0;
							i++;
							struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
						}else if(tokens[i]->type == A_IDENTIFIER){
							new_instruction->identifier_token = tokens[i];
							new_instruction->number_token = 0;
							verify_symbol_declaration(linker_object, tokens[i]);
							struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
							i++;
						}else{ assert(0 && "Expected identifier or hexidecimal constant."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_DW){
			struct asm_instruction * new_instruction = malloc(sizeof(struct asm_instruction));
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_CONSTANT_HEX){
					new_instruction->number_token_is_negative = 0;
					new_instruction->number_token = tokens[i];
					new_instruction->identifier_token = 0;
					i++;
					struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
				}else if(tokens[i]->type == A_IDENTIFIER){
					new_instruction->identifier_token = tokens[i];
					new_instruction->number_token = 0;
					verify_symbol_declaration(linker_object, tokens[i]);
					struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
					i++;
				}else{ assert(0 && "Expected identifier or hexidecimal constant."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_DIV){
			struct asm_instruction * new_instruction = malloc(sizeof(struct asm_instruction));
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_REGISTER){
							new_instruction->ry_token = tokens[i];
							i++;
							if(tokens[i]->type == A_SPACE){
								i++;
								if(tokens[i]->type == A_REGISTER){
									new_instruction->rz_token = tokens[i];
									i++;
									if(tokens[i]->type == A_SPACE){
										i++;
										if(tokens[i]->type == A_REGISTER){
											new_instruction->rw_token = tokens[i];
											i++;
											struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
										}else{ assert(0 && "Expected register."); }
									}else{ assert(0 && "Expected space."); }
								}else{ assert(0 && "Expected register."); }
							}else{ assert(0 && "Expected space."); }
						}else{ assert(0 && "Expected register."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_BEQ || tokens[i]->type == A_BLT){
			struct asm_instruction * new_instruction = malloc(sizeof(struct asm_instruction));
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_REGISTER){
							new_instruction->ry_token = tokens[i];
							i++;
							if(tokens[i]->type == A_SPACE){
								i++;
								if(tokens[i]->type == A_IDENTIFIER){
									new_instruction->identifier_token = tokens[i];
									new_instruction->number_token = 0;
									new_instruction->number_token_is_negative = 0;
									verify_symbol_declaration(linker_object, tokens[i]);
									struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
									i++;

								}else if(tokens[i]->type == A_MINUS_CHAR){
									i++;
									if(tokens[i]->type == A_CONSTANT_DECIMAL){
										new_instruction->number_token = tokens[i];
										new_instruction->identifier_token = 0;
										new_instruction->number_token_is_negative = 1;
										i++;
										struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
									}else{ assert(0 && "Expected number."); }
								}else if(tokens[i]->type == A_CONSTANT_DECIMAL){
									new_instruction->number_token = tokens[i];
									new_instruction->identifier_token = 0;
									new_instruction->number_token_is_negative = 0;
									i++;
									struct_asm_instruction_ptr_list_add(&linker_object->instructions, new_instruction);
								}else{ assert(0 && "Expected number or identifier."); }
							}else{ assert(0 && "Expected space."); }
						}else{ assert(0 && "Expected register."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_OFFSET){
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_RELOCATABLE){
					i++;
					linker_object->is_relocatable = 1; /* All symbols are relative */
				}else if(tokens[i]->type == A_CONSTANT_HEX){
					i++;
					assert(0 && "Cannot link using non relocatable file.");
				}else{ assert(0 && "Expected space."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_REQUIRES){
			i++;
			if(tokens[i]->type == A_COMMA_CHAR){
				i++;
				if(tokens[i]->type == A_SPACE){
					i++;
					if(tokens[i]->type == A_IMPLEMENTS){
						i++;
						if(tokens[i]->type == A_SPACE){
							i++;
							if(tokens[i]->type == A_INTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_linker_symbol(linker_object, tokens[i], 0, 1, 1);
										i++;
									}else{ assert(0 && "Expected identifier."); }
								}else{ assert(0 && "Expected space."); }
							}else if(tokens[i]->type == A_EXTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_linker_symbol(linker_object, tokens[i], 1, 1, 1);
										i++;
									}else{ assert(0 && "Expected identifier."); }
								}else{ assert(0 && "Expected space."); }
							}else{ assert(0 && "Expected INTERNAL or EXTERNAL."); }

						}else{ assert(0 && "Expected space."); }
					}else{ assert(0 && "Expected REQUIRES."); }
				}else{ assert(0 && "Expected space."); }
			}else if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_INTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_linker_symbol(linker_object, tokens[i], 0, 1, 0);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else if(tokens[i]->type == A_EXTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_linker_symbol(linker_object, tokens[i], 1, 1, 0);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected INTERNAL or EXTERNAL."); }
			}else{ assert(0 && "Expected space or comma."); }
		}else if(tokens[i]->type == A_IMPLEMENTS){
			i++;
			if(tokens[i]->type == A_COMMA_CHAR){
				i++;
				if(tokens[i]->type == A_SPACE){
					i++;
					if(tokens[i]->type == A_REQUIRES){
						i++;
						if(tokens[i]->type == A_SPACE){
							i++;
							if(tokens[i]->type == A_INTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_linker_symbol(linker_object, tokens[i], 0, 1, 1);
										i++;
									}
								}else{ assert(0 && "Expected space."); }
							}else if(tokens[i]->type == A_EXTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_linker_symbol(linker_object, tokens[i], 1, 1, 1);
										i++;
									}
								}else{ assert(0 && "Expected space."); }
							}else{ printf("On line %d\n", linker_object->current_line); assert(0 && "Expected INTERNAL or EXTERNAL."); }
						}else{ assert(0 && "Expected space."); }
					}else{ assert(0 && "Expected REQUIRES."); }
				}else{ assert(0 && "Expected space."); }
			}else if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_INTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_linker_symbol(linker_object, tokens[i], 0, 0, 1);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else if(tokens[i]->type == A_EXTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_linker_symbol(linker_object, tokens[i], 1, 0, 1);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else{ printf("On line %d\n", linker_object->current_line); assert(0 && "Expected INTERNAL or EXTERNAL."); }
			}else{ assert(0 && "Expected space or comma."); }
		}else if(tokens[i]->type == A_ASM_COMMENT){
			i++;
		}else if(tokens[i]->type == A_NEWLINE){
			i++;
			linker_object->current_line = linker_object->current_line + 1;
		}else if(tokens[i]->type == A_IDENTIFIER){
			struct asm_lexer_token * identifier = tokens[i];
			i++;
			if(tokens[i]->type == A_COLON_CHAR){
				set_symbol_instruction_index(linker_object, identifier, struct_asm_instruction_ptr_list_size(&linker_object->instructions));
				i++;
			}else{ printf("On line %d in file %s\n", linker_object->current_line, asm_lexer_state->c.filename); assert(0 && "Expected colon."); }
		}else{
			assert(0 && "Unable to process assembly file.");
		}
	}
	return linker_object;
}

void buffered_token_output(struct unsigned_char_list * buffer, struct asm_lexer_token * t){
	unsigned char * c = t->first_byte;
	while(c != t->last_byte) {
		buffered_printf(buffer, "%c", *c);
		c++;
	}
	buffered_printf(buffer, "%c", *c);
}

unsigned int get_absolute_symbol_offset(unsigned char * identifier, struct linker_object * current_linker_object, struct struct_linker_object_ptr_list * all_linker_objects){
	struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&current_linker_object->symbols, identifier);
	if(symbol && symbol->is_implemented){
		return struct_asm_instruction_ptr_list_get(&current_linker_object->instructions, symbol->instruction_index)->post_linking_offset + current_linker_object->linker_object_post_linking_offset;
	}else{
		unsigned int i;
		for(i = 0; i < struct_linker_object_ptr_list_size(all_linker_objects); i++){
			struct linker_object * obj = struct_linker_object_ptr_list_get(all_linker_objects, i);
			struct linker_symbol * external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&obj->symbols, identifier);
			if(external_symbol && external_symbol->is_external && external_symbol->is_implemented){
				return struct_asm_instruction_ptr_list_get(&obj->instructions, external_symbol->instruction_index)->post_linking_offset + obj->linker_object_post_linking_offset;
			}
		}
		printf("Symbol %s was not found in any linker object.  Symbol referenced in file %s.\n", identifier, current_linker_object->asm_lexer_state->c.filename);
		assert(0 && "Symbol was not found in any linker object.");
	}
}

void output_artifacts(struct unsigned_char_list * file_output, struct linker_object * linker_object, struct struct_linker_object_ptr_list * all_linker_objects, struct unsigned_char_list * symbol_file){
	struct struct_asm_instruction_ptr_list * instructions = &linker_object->instructions;
	unsigned int size = struct_asm_instruction_ptr_list_size(instructions);
	struct asm_instruction ** data = struct_asm_instruction_ptr_list_data(instructions);
	unsigned int i;

	if(!linker_object->is_relocatable){
		printf("File %s does not specify that symbols are relocatable.", linker_object->asm_lexer_state->c.filename);
		assert(0 && "Symbols not relocatable.");
	}

	for(i = 0; i < size; i++){
		struct asm_instruction * instruction = data[i];
		enum asm_token_type type = instruction->op_token->type;

		if (type == A_ADD || type == A_SUB || type == A_MUL || type == A_AND || type == A_OR){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rx_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->ry_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rz_token);
			buffered_printf(file_output, "\n");
		}else if(type == A_LOA || type == A_STO || type == A_NOT || type == A_SHR || type == A_SHL){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rx_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->ry_token);
			buffered_printf(file_output, "\n");
		}else if (type == A_DIV){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rx_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->ry_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rz_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rw_token);
			buffered_printf(file_output, "\n");
		}else if(type == A_LL){
			if(instruction->identifier_token){
				/*  ll instruction cannot load all 32 bit addresses directoy, so re-writing identifier based ll statements to handle this case  */
				unsigned int possibly_uses_r2 = instruction->rx_token->first_byte[1] == '2';
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte);
				unsigned int absolute_offset = get_absolute_symbol_offset(ident, linker_object, all_linker_objects);
				unsigned char * target_register = copy_string(instruction->rx_token->first_byte, instruction->rx_token->last_byte);
				unsigned char * temp_register = possibly_uses_r2 ? (unsigned char *)"r1" : (unsigned char *)"r2";
				free(ident);
				buffered_printf(file_output, "sub SP SP WR\n");
				buffered_printf(file_output, "sto SP %s\n", temp_register);
				buffered_printf(file_output, "ll %s 0x%X\n", target_register, (0xFFFF0000 & (absolute_offset * 4)) >> 16);
				buffered_printf(file_output, "ll %s 0x10\n", temp_register);
				buffered_printf(file_output, "shl %s %s\n", target_register, temp_register);
				buffered_printf(file_output, "ll %s 0x%X\n", temp_register, 0xFFFF & (absolute_offset * 4));
				buffered_printf(file_output, "or %s %s %s\n", target_register, target_register, temp_register);
				buffered_printf(file_output, "loa %s SP\n", temp_register);
				buffered_printf(file_output, "add SP SP WR\n");
				free(target_register);
			}else{
				buffered_token_output(file_output, instruction->op_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->number_token);
				buffered_printf(file_output, "\n");
			}
		}else if(type == A_BEQ || type == A_BLT){
			if(instruction->identifier_token){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte);
				unsigned int absolute_offset = get_absolute_symbol_offset(ident, linker_object, all_linker_objects);
				free(ident);
				buffered_token_output(file_output, instruction->op_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->ry_token);
				buffered_printf(file_output, " ");
				buffered_printf(file_output, "1");
				buffered_printf(file_output, "\n");
				buffered_printf(file_output, "beq ZR ZR 2\n"); /* If we're not going to branch, skip the 2 far jump instructions */
				buffered_printf(file_output, "loa PC PC\n");
				buffered_printf(file_output, "dw 0x%X\n", absolute_offset * 4);
			}else{
				buffered_token_output(file_output, instruction->op_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->ry_token);
				buffered_printf(file_output, " ");
				if(instruction->number_token_is_negative){
					buffered_printf(file_output, "-");
				}
				buffered_token_output(file_output, instruction->number_token);
				buffered_printf(file_output, "\n");
			}
		}else if(type == A_DW){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			if(instruction->identifier_token){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte);
				unsigned int absolute_offset = get_absolute_symbol_offset(ident, linker_object, all_linker_objects);
				buffered_printf(file_output, "0x%X", absolute_offset * 4);
				free(ident);
			}else{
				buffered_token_output(file_output, instruction->number_token);
			}
			buffered_printf(file_output, "\n");
		}else{
			assert(0 && "Unknown instruction type.");
		}
	}

	if(symbol_file){
		struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&linker_object->symbols);
		unsigned int keys_size = unsigned_char_ptr_list_size(&keys);
		unsigned int g;
		buffered_printf(symbol_file, "Symbols for file %s:\n", linker_object->asm_lexer_state->c.filename);
		for(g = 0; g < keys_size; g++){
			unsigned char * id = unsigned_char_ptr_list_get(&keys, g);
			unsigned int absolute_offset = get_absolute_symbol_offset(id, linker_object, all_linker_objects);
			buffered_printf(symbol_file, "0x%08X %s\n", absolute_offset * 4, id);
		}
		unsigned_char_ptr_list_destroy(&keys);
	}
}

void free_symbol_map(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(map);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, unsigned_char_ptr_list_get(&keys, i)));
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_destroy(map);
	unsigned_char_ptr_list_destroy(&keys);
}

int do_link(struct memory_pooler_collection * memory_pooler_collection, struct unsigned_char_ptr_list * in_files, unsigned char * out_file, unsigned char * symbol_file){
	struct struct_unsigned_char_list_ptr_list input_file_buffers;
	struct struct_linker_object_ptr_list linker_objects;
	struct struct_asm_lexer_state_ptr_list lexer_states;
	struct unsigned_char_list file_output;
	struct unsigned_char_list symbol_output;
	struct unsigned_char_list asm_lexer_output;
	unsigned int i;
	unsigned int next_linker_object_post_linking_offset = 0;

	g_format_buffer_use();

	unsigned_char_list_create(&file_output);
	unsigned_char_list_create(&symbol_output);
	unsigned_char_list_create(&asm_lexer_output);
	struct_unsigned_char_list_ptr_list_create(&input_file_buffers);
	struct_asm_lexer_state_ptr_list_create(&lexer_states);
	struct_linker_object_ptr_list_create(&linker_objects);


	for(i = 0; i < unsigned_char_ptr_list_size(in_files); i++){
		struct unsigned_char_list * file_input = malloc(sizeof(struct unsigned_char_list));
		struct asm_lexer_state * asm_lexer_state = malloc(sizeof(struct asm_lexer_state));
		struct linker_object * linker_object;
		unsigned_char_list_create(file_input);
		add_file_to_buffer(file_input, (char*)unsigned_char_ptr_list_get(in_files, i));

		asm_lexer_state->c.memory_pooler_collection = memory_pooler_collection;
		asm_lexer_state->c.buffered_output = &asm_lexer_output;

		lex_asm(asm_lexer_state, unsigned_char_ptr_list_get(in_files, i), unsigned_char_list_data(file_input), unsigned_char_list_size(file_input));
		linker_object = process_assembly(asm_lexer_state);
		linker_object->linker_object_post_linking_offset = next_linker_object_post_linking_offset;
		next_linker_object_post_linking_offset += set_post_linking_offsets(linker_object); 

		struct_unsigned_char_list_ptr_list_add(&input_file_buffers, file_input);
		struct_linker_object_ptr_list_add(&linker_objects, linker_object);
		struct_asm_lexer_state_ptr_list_add(&lexer_states, asm_lexer_state);
	}

	/* TODO:  Allow the user to specify the offset */
	buffered_printf(&file_output, "OFFSET 0x0\n");

	for(i = 0; i < struct_linker_object_ptr_list_size(&linker_objects); i++){
		output_artifacts(&file_output, struct_linker_object_ptr_list_get(&linker_objects, i), &linker_objects, &symbol_output);
	}

	/*  Clean up all the resources */
	for(i = 0; i < unsigned_char_ptr_list_size(in_files); i++){
		unsigned int j;
		struct unsigned_char_list * file_input = struct_unsigned_char_list_ptr_list_get(&input_file_buffers, i);
		struct linker_object * linker_object = struct_linker_object_ptr_list_get(&linker_objects, i);
		struct asm_lexer_state * asm_lexer_state = struct_asm_lexer_state_ptr_list_get(&lexer_states, i);
		unsigned_char_list_destroy(file_input);
		free(file_input);

		free_symbol_map(&linker_object->symbols);

		for(j = 0; j < struct_asm_instruction_ptr_list_size(&linker_object->instructions); j++){
			free(struct_asm_instruction_ptr_list_get(&linker_object->instructions, j));
		}
		struct_asm_instruction_ptr_list_destroy(&linker_object->instructions);
		free(linker_object);

		destroy_asm_lexer_state(asm_lexer_state);
		free(asm_lexer_state);
	}

	/*  Output to a file our final product with all objects linked together */
	output_buffer_to_file(&file_output, (char*)out_file);
	if(symbol_file){
		output_buffer_to_file(&symbol_output, (char*)symbol_file);
	}

	unsigned_char_list_destroy(&asm_lexer_output);
	unsigned_char_list_destroy(&file_output);
	unsigned_char_list_destroy(&symbol_output);
	struct_asm_lexer_state_ptr_list_destroy(&lexer_states);
	struct_unsigned_char_list_ptr_list_destroy(&input_file_buffers);
	struct_linker_object_ptr_list_destroy(&linker_objects);
	g_format_buffer_release();

	return EXIT_SUCCESS;
}