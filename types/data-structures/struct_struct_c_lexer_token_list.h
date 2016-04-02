#ifndef TYPES_DATA_STRUCTURES_struct_struct_c_lexer_token_list_H_
#define TYPES_DATA_STRUCTURES_struct_struct_c_lexer_token_list_H_
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

#ifndef BINARY_EXPONENTIAL_BUFFER_H_DEFINED_
#include "../../data-structures/binary_exponential_buffer.h"
#endif
#ifndef TYPES_LEXER_struct_c_lexer_token_H_
#include "../../types/lexer/struct_c_lexer_token.h"
#endif

struct struct_c_lexer_token_list{
	struct binary_exponential_buffer buffer;
};

#endif