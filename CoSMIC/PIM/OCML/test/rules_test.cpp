#include <engine/option_reference.hpp>
#include <engine/option_rules.hpp>
#include <engine/logic_rules.hpp>

#include "rules_test.hpp"

#include <string>
#include <map>

template <typename OPTION_TRAIT>
bool try_write_info(OCML::Option_Reference* reference)
{
  typedef OCML::Basic_Option_Reference<OPTION_TRAIT> reference_type;
  reference_type* basic_reference = dynamic_cast<reference_type*>(reference);

  if (basic_reference)
  {
    write_info(basic_reference);
  }
 
  bool result = basic_reference != 0;

  return result;
}

template <typename OPTION_TRAIT>
std::string* try_str_info(OCML::Option_Reference* reference)
{
  std::string* result = 0;

  typedef OCML::Basic_Option_Reference<OPTION_TRAIT> reference_type;
  reference_type* basic_reference = dynamic_cast<reference_type*>(reference);

  if (basic_reference)
  {
    result = new std::string(str_info(basic_reference));
  }
 
  return result;
}

references_map* create_references(OCML::Option_Category* category)
{
  using OCML::Option_Category;
  using OCML::Option_Reference_Factory;

  references_map* result = new references_map;

  for (Option_Category::option_iterator iter = category->begin_options();
       iter != category->end_options(); ++iter)
  {
    (*result)[*iter] =
      Option_Reference_Factory::create_option_reference(*iter);
  }

  return result;
}

void clear_references(references_map* references)
{
  for (references_map::iterator iter = references->begin();
       iter != references->end(); ++iter)
  {
    iter->second->clear();
  }
}

void write_info(OCML::Option_Reference* reference)
{
  bool result = false;
  
  if (!result)
    result = try_write_info<OCML::Boolean_Option_Trait>(reference);
  if (!result)
    result = try_write_info<OCML::Integer_Option_Trait>(reference);
  if (!result)
    result = try_write_info<OCML::String_Option_Trait>(reference);
  if (!result)
    std::cerr << "Cannot downcast the option reference.\n";
}

std::string str_info(OCML::Option_Reference* reference)
{
  std::string* result = 0;
  
  if (result == 0)
    result = try_str_info<OCML::Boolean_Option_Trait>(reference);
  if (result == 0)
    result = try_str_info<OCML::Integer_Option_Trait>(reference);
  if (result == 0)
    result = try_str_info<OCML::String_Option_Trait>(reference);
  if (result == 0)
    std::cerr << "Cannot downcast the option reference.\n";

  std::string return_value(*result);
  delete result;
  return return_value;
}

void write_info(references_map* references)
{
  for (references_map::iterator iter = references->begin();
       iter != references->end(); ++iter)
  {
    write_info(iter->second);
  }
}

