#include "definitions_test.hpp"

#include <iostream>

void write_info(OCML::Option_Definition* option)
{
  std::cout << "Option: " << std::endl
            << "  name: " << option->name() << std::endl
            << "  type: " << option->type_string() << std::endl
            << "  description: " << option->description() << std::endl;
}

void write_info(OCML::Option_Category* category)
{
  for (OCML::Option_Category::option_iterator iter = category->begin_options();
       iter != category->end_options(); ++iter)
  {
    write_info(*iter);
  }
}

std::string str_info(OCML::Option_Definition* option)
{
  std::stringstream s;
  s << option->name() << '<' << option->type_string() << '>';
  return s.str();
}

std::string str_info(OCML::Option_Category* category)
{
  std::stringstream s;

  s << category->name() << " {";

  OCML::Option_Category::option_iterator iter = category->begin_options();
  if (iter != category->end_options())
  {
    s << str_info(*iter);
    ++iter;
  }

  for (; iter != category->end_options(); ++iter)
  {
    s << ", " << str_info(*iter);
  }

  s << '}';
  return s.str();
}

OCML::Option_Category* create_category()
{
  OCML::Option_Category* result =
    new OCML::Option_Category("category_def_test", "Test option category");

  result->add_option(
      create_definition<OCML::Integer_Option_Definition>(
                             "int_def_test",
                             "Test option for integer option definition",
                             0));

  result->add_option(
      create_definition<OCML::String_Option_Definition>(
                             "str_def_test",
                             "Test option for string option definition",
                             ""));

  result->add_option(
      create_definition<OCML::Boolean_Option_Definition>(
                             "bool_def_test",
                              "Test option for boolean option definition",
                              false));

  return result;
}

