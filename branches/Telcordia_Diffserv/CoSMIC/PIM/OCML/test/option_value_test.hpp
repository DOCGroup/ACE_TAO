#ifndef _OPTION_VALUE_TEST_HPP_
#define _OPTION_VALUE_TEST_HPP_

#include <engine/option_value.hpp>
#include <engine/definitions.hpp>
#include <map>
#include "value_test.hpp"
#include "definitions_test.hpp"

template <typename OPTION_VALUE_TYPE>
OCML::Basic_Option_Value<OPTION_VALUE_TYPE>*
try_create_option_value(OCML::Option_Definition* option);

OCML::Option_Value*
create_option_value(OCML::Option_Definition* option);

typedef std::map<OCML::Option_Definition*, OCML::Option_Value*> 
  option_value_map;

option_value_map* create_values(OCML::Option_Category* category);

void delete_values(option_value_map* values);

void clear_values(option_value_map* values);

void set_values(option_value_map* values);

void write_info(option_value_map* values);

#include "option_value_test.ipp"
#include "option_value_test.tpp"

#endif // _OPTION_VALUE_TEST_HPP_
