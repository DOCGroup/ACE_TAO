#include "option_value_test.hpp"
#include "definitions_test.hpp"

#include <iostream>

int main(int, char*[])
{
  std::auto_ptr<OCML::Option_Category> category(create_category());

  std::cout << "Creating values for the options:\n";
  write_info(category.get());

  std::auto_ptr<option_value_map> values(create_values(category.get()));

  std::cout << "Values for options:\n";
  write_info(values.get());

  std::cout << "Clear values\n";
  clear_values(values.get());
  std::cout << "After clear:\n";
  write_info(values.get());
  std::cout << "Set values\n";
  set_values(values.get());
  std::cout << "After set:\n";
  write_info(values.get());

  delete_values(values.get());
 
  return 0;
}
