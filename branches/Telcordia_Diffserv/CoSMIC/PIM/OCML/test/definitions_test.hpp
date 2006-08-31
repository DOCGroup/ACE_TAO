#ifndef _DEFINITIONS_TEST_HPP_
#define _DEFINITIONS_TEST_HPP_

#include <engine/definitions.hpp>
#include <string>

template <typename OPTION_TYPE>
OPTION_TYPE* create_definition(const char* name, const char* descr,
                               typename OPTION_TYPE::value_type value);

OCML::Option_Category* create_category();

void write_info(OCML::Option_Definition* option);

void write_info(OCML::Option_Category* category);

std::string str_info(OCML::Option_Definition* option);

std::string str_info(OCML::Option_Category* category);

#include "definitions_test.tpp"

#endif // _DEFINITIONS_TEST_HPP_
