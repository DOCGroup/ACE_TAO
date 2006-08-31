#include "value_test.hpp"
#include <iostream>
#include "test.hpp"

int main(int, char*[])
{
  // Some values
  OCML::Integer_Option_Value int_value(3);
  OCML::Boolean_Option_Value bool_value(true);
  OCML::String_Option_Value  str_value("test");

  std::cout << "Some values:" << std::endl;
  write_info(&int_value);
  write_info(&bool_value);
  write_info(&str_value);
  std::cout << std::endl;
 
  // Default values
  OCML::Integer_Option_Value def_int_value;
  OCML::Boolean_Option_Value def_bool_value;
  OCML::String_Option_Value  def_str_value;

  std::cout << "Default values:" << std::endl;
  write_info(&def_int_value);
  write_info(&def_bool_value);
  write_info(&def_str_value);
  std::cout << std::endl;

  // Check equality
  std::cout << "int(NULL) == int(3)" << std::endl;
  write_bool_str(def_int_value.compare(&int_value));
  std::cout << std::endl << std::endl;
 
  // Assignment
  def_int_value.set(&int_value);
  def_bool_value.set(&bool_value);
  def_str_value.set(&str_value);

  std::cout << "After assignment:" << std::endl;
  write_info(&def_int_value);
  write_info(&def_bool_value);
  write_info(&def_str_value);
  std::cout << std::endl;

  // Check equality
  std::cout << "int(3) == int(3)" << std::endl;
  write_bool_str(def_int_value.compare(&int_value));
  std::cout << std::endl << std::endl;
  
  // Clear
  def_int_value.clear();
  def_bool_value.clear();
  def_str_value.clear();

  std::cout << "After clear:" << std::endl;
  write_info(&def_int_value);
  write_info(&def_bool_value);
  write_info(&def_str_value);
  std::cout << std::endl;

  // Value type mismatch.
  try {
    std::cout << "integer_value = boolean_value" << std::endl;
    def_int_value.set(&bool_value);
  }
  catch (OCML::Value_Type_Mismatch&) {
    std::cout << "Value type mismatch exception cought." << std::endl;
    std::cout << std::endl;
  }

  try {
    std::cout << "integer_value == boolean_value" << std::endl;
    def_int_value.compare(&bool_value);
  }
  catch (OCML::Value_Type_Mismatch&) {
    std::cout << "Value type mismatch exception cought." << std::endl;
    std::cout << std::endl;
  }

  return 0;
}
