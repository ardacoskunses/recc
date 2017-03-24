

/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/

/*GETS_REPLACED_WITH_INCLUDES*/

struct asm_state{
	struct enum_asm_operation_type_list reversible_operations;
	struct struct_aggregate_type_specifier_details_list aggregate_type_specifiers;
	struct struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map aggregate_type_specifiers_map;
	struct struct_simple_type_specifier_details_list simple_type_specifiers;
	struct struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map simple_type_specifiers_map;
	struct struct_simple_type_qualifier_details_list simple_type_qualifiers;
	struct struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map simple_type_qualifiers_map;
	struct struct_simple_storage_class_specifier_details_list simple_storage_class_specifiers;
	struct struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map simple_storage_class_specifiers_map;
	struct struct_specifier_or_qualifier_details_list specifier_or_qualifiers;
	struct struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map specifier_or_qualifiers_map;
	struct struct_specifier_or_qualifier_list_item_details_list specifier_or_qualifier_list_items;
	struct struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map specifier_or_qualifier_list_items_map;
	struct struct_parameter_list_details_list parameter_lists;
	struct struct_parameter_list_details_to_struct_parameter_list_id_map parameter_lists_map;
	struct struct_declarator_part_details_list declarator_parts;
	struct struct_declarator_part_details_to_struct_declarator_part_id_map declarator_parts_map;
	struct struct_any_statement_list_item_details_list any_statement_list_items;
	struct struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map any_statement_list_items_map;
	struct struct_anonymous_tag_definition_details_list anonymous_tag_definitions;
	struct struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map anonymous_tag_definitions_map;
	struct struct_named_tag_predeclaration_details_list named_tag_predeclarations;
	struct struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map named_tag_predeclarations_map;
	struct struct_named_tag_definition_details_list named_tag_definitions;
	struct struct_named_tag_definition_details_to_struct_named_tag_definition_id_map named_tag_definitions_map;
	struct struct_tag_definition_details_list tag_definitions;
	struct struct_tag_definition_details_to_struct_tag_definition_id_map tag_definitions_map;
	struct struct_struct_or_union_member_list_item_details_list struct_or_union_member_list_items;
	struct struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map struct_or_union_member_list_items_map;
	struct struct_enum_member_list_item_details_list enum_member_list_items;
	struct struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map enum_member_list_items_map;
	struct struct_enum_member_details_list enum_members;
	struct struct_enum_member_details_to_struct_enum_member_id_map enum_members_map;
	struct struct_struct_or_union_member_details_list struct_or_union_members;
	struct struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map struct_or_union_members_map;
	struct struct_initializer_details_list initializers;
	struct struct_initializer_details_to_struct_initializer_id_map initializers_map;
	struct struct_initializer_list_item_details_list initializer_list_items;
	struct struct_initializer_list_item_details_to_struct_initializer_list_item_id_map initializer_list_items_map;
	struct struct_declarator_part_list_item_details_list declarator_part_list_items;
	struct struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map declarator_part_list_items_map;
	struct struct_pointer_part_details_list pointer_parts;
	struct struct_pointer_part_details_to_struct_pointer_part_id_map pointer_parts_map;
	struct struct_function_part_details_list function_parts;
	struct struct_function_part_details_to_struct_function_part_id_map function_parts_map;
	struct struct_array_part_details_list array_parts;
	struct struct_array_part_details_to_struct_array_part_id_map array_parts_map;
	struct struct_general_type_details_list general_types;
	struct struct_general_type_details_to_struct_general_type_id_map general_types_map;
	struct struct_identifier_namespace_entry_details_list identifier_namespace_entrys;
	struct struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map identifier_namespace_entrys_map;
	struct struct_general_type_list_item_details_list general_type_list_items;
	struct struct_general_type_list_item_details_to_struct_general_type_list_item_id_map general_type_list_items_map;
	struct struct_postfix_expression_part_list_item_details_list postfix_expression_part_list_items;
	struct struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map postfix_expression_part_list_items_map;
	struct struct_postfix_expression_part_details_list postfix_expression_parts;
	struct struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map postfix_expression_parts_map;
	struct struct_unscoped_tag_specifier_details_list unscoped_tag_specifiers;
	struct struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map unscoped_tag_specifiers_map;
	struct struct_scoped_typename_specifier_details_list scoped_typename_specifiers;
	struct struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map scoped_typename_specifiers_map;
	struct struct_scoped_tag_specifier_details_list scoped_tag_specifiers;
	struct struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map scoped_tag_specifiers_map;
	struct struct_unscoped_union_specifier_details_list unscoped_union_specifiers;
	struct struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map unscoped_union_specifiers_map;
	struct struct_unscoped_struct_specifier_details_list unscoped_struct_specifiers;
	struct struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map unscoped_struct_specifiers_map;
	struct struct_unscoped_enum_specifier_details_list unscoped_enum_specifiers;
	struct struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map unscoped_enum_specifiers_map;
	struct struct_bitfield_details_list bitfields;
	struct struct_bitfield_details_to_struct_bitfield_id_map bitfields_map;
	struct struct_jump_statement_details_list jump_statements;
	struct struct_jump_statement_details_to_struct_jump_statement_id_map jump_statements_map;
	struct struct_switch_statement_details_list switch_statements;
	struct struct_switch_statement_details_to_struct_switch_statement_id_map switch_statements_map;
	struct struct_if_statement_details_list if_statements;
	struct struct_if_statement_details_to_struct_if_statement_id_map if_statements_map;
	struct struct_while_statement_details_list while_statements;
	struct struct_while_statement_details_to_struct_while_statement_id_map while_statements_map;
	struct struct_for_statement_details_list for_statements;
	struct struct_for_statement_details_to_struct_for_statement_id_map for_statements_map;
	struct struct_code_statement_details_list code_statements;
	struct struct_code_statement_details_to_struct_code_statement_id_map code_statements_map;
	struct struct_labeled_statement_details_list labeled_statements;
	struct struct_labeled_statement_details_to_struct_labeled_statement_id_map labeled_statements_map;
	struct struct_postfix_expression_details_list postfix_expressions;
	struct struct_postfix_expression_details_to_struct_postfix_expression_id_map postfix_expressions_map;
	struct struct_primary_expression_details_list primary_expressions;
	struct struct_primary_expression_details_to_struct_primary_expression_id_map primary_expressions_map;
	struct struct_any_statement_details_list any_statements;
	struct struct_any_statement_details_to_struct_any_statement_id_map any_statements_map;
	struct struct_scoped_statement_details_list scoped_statements;
	struct struct_scoped_statement_details_to_struct_scoped_statement_id_map scoped_statements_map;
	struct struct_declaration_statement_details_list declaration_statements;
	struct struct_declaration_statement_details_to_struct_declaration_statement_id_map declaration_statements_map;
	struct struct_function_definition_details_list function_definitions;
	struct struct_function_definition_details_to_struct_function_definition_id_map function_definitions_map;
	struct struct_simple_declaration_details_list simple_declarations;
	struct struct_simple_declaration_details_to_struct_simple_declaration_id_map simple_declarations_map;
	struct struct_expression_details_list expressions;
	struct struct_expression_details_to_struct_expression_id_map expressions_map;
	struct struct_cast_expression_details_list cast_expressions;
	struct struct_cast_expression_details_to_struct_cast_expression_id_map cast_expressions_map;
	struct struct_unary_expression_details_list unary_expressions;
	struct struct_unary_expression_details_to_struct_unary_expression_id_map unary_expressions_map;
	struct struct_binary_expression_details_list binary_expressions;
	struct struct_binary_expression_details_to_struct_binary_expression_id_map binary_expressions_map;
	struct struct_ternary_expression_details_list ternary_expressions;
	struct struct_ternary_expression_details_to_struct_ternary_expression_id_map ternary_expressions_map;
	struct struct_constant_description_details_list constant_descriptions;
	struct struct_constant_description_details_to_struct_constant_description_id_map constant_descriptions_map;
	struct struct_data_package_details_list data_packages;
	struct struct_data_package_details_to_struct_data_package_id_map data_packages_map;
	struct struct_error_node_details_list error_nodes;
	struct struct_error_node_details_to_struct_error_node_id_map error_nodes_map;
	struct struct_byte_package_details_list byte_packages;
	struct struct_byte_package_details_to_struct_byte_package_id_map byte_packages_map;
	struct struct_hword_package_details_list hword_packages;
	struct struct_hword_package_details_to_struct_hword_package_id_map hword_packages_map;
	struct struct_word_package_details_list word_packages;
	struct struct_word_package_details_to_struct_word_package_id_map word_packages_map;
	struct struct_qword_package_details_list qword_packages;
	struct struct_qword_package_details_to_struct_qword_package_id_map qword_packages_map;
	struct struct_dword_package_details_list dword_packages;
	struct struct_dword_package_details_to_struct_dword_package_id_map dword_packages_map;
	struct struct_string_package_details_list string_packages;
	struct struct_string_package_details_to_struct_string_package_id_map string_packages_map;
	struct struct_identifier_details_list identifiers;
	struct struct_identifier_details_to_struct_identifier_id_map identifiers_map;
	struct struct_scope_guid_details_list scope_guids;
	struct struct_scope_guid_details_to_struct_scope_guid_id_map scope_guids_map;
	struct struct_anonymous_entry_details_list anonymous_entrys;
	struct struct_anonymous_entry_details_to_struct_anonymous_entry_id_map anonymous_entrys_map;
	unsigned int next_scope_guid;
	unsigned int next_anonymous_entry;
};

