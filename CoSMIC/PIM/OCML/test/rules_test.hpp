#ifndef _RULES_TEST_HPP_
#define _RULES_TEST_HPP_

#include <map>
#include <string>

#include <engine/definitions.hpp>
#include <engine/option_reference.hpp>

typedef std::map<OCML::Option_Definition*, OCML::Option_Reference*>
  references_map;

template <class OPTION_TRAIT>
void write_info(OCML::Basic_Option_Reference<OPTION_TRAIT>* option);

template <class OPTION_TRAIT>
std::string str_info(OCML::Basic_Option_Reference<OPTION_TRAIT>* option);

references_map* create_references(OCML::Option_Category* category);
  
void clear_references(references_map* references);

void write_info(OCML::Option_Reference* reference);

std::string str_info(OCML::Option_Reference* reference);

void write_info(references_map* references);

#include "rules_test.tpp"

#endif // _RULES_TEST_HPP_
