#ifndef TYPES_LINKER_enum_relocation_type_h_
#define TYPES_LINKER_enum_relocation_type_h_
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

enum relocation_type{
	RELOCATION_TYPE_RELOCATABLE,
	RELOCATION_TYPE_END,
	RELOCATION_TYPE_ADDRESS
};

#endif