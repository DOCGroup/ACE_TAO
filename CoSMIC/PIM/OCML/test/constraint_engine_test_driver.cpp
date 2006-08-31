#include "constraint_engine_test.hpp"

int main(int, char*[])
{
  bool result;

  // Create definitions
  std::cout << "Creating category..." << std::endl;
  std::auto_ptr<OCML::Option_Category> category(create_category());
  // Add another option
  OCML::Integer_Option_Definition* new_option =
    new OCML::Integer_Option_Definition("new_option",
                                        "test option for if rule");
  category->add_option(new_option);
  std::cout << "Created: " << str_info(category.get()) << std::endl;
  std::cout << std::endl;

  // Create constraint engine
  std::cout << "Creating constraint engine..." << std::endl;
  OCML::Constraint_Engine engine(category.get());
  std::cout << "Created." << std::endl;
  std::cout << std::endl;

  // Create listeners and register them.
  std::cout << "Creating listeners and binding them..." << std::endl;
  std::auto_ptr<listener_map> listeners(
    create_listeners(category.get(), engine));
  std::cout << "Created and binded." << std::endl;
  std::cout << std::endl;

  // Create some predefined rules.
  std::cout << "Creating if rule: (int_def_test=3) ==> (new_option=8)\n";
  std::auto_ptr<OCML::Integer_Option_Value> int_value_8(
    new OCML::Integer_Option_Value(8));
  OCML::Constraint_Engine::rule_index then_rule =
    engine.define_equality_rule(new_option, int_value_8.get());

  std::auto_ptr<OCML::Integer_Option_Value> int_value_3(
    new OCML::Integer_Option_Value(3));
  OCML::Constraint_Engine::rule_index if_rule =
    engine.define_equality_rule(category->option("int_def_test"),
                                int_value_3.get());

  OCML::Constraint_Engine::rule_index new_rule =
    engine.define_if_rule(if_rule, then_rule);
  engine.add_rule(new_rule);
  std::cout << "Created if rule, validating...\n";
  result = engine.validate();
  std::cout << "Validated. Result: ";
  if (result)
    std::cout << "true\n";
  else
    std::cout << "false\n";

  // Assign values to options.
  std::cout << "Assigning values by defining rules..." << std::endl;
  assign_values(category.get(), engine); 
  std::cout << "Assigned." << std::endl;
  std::cout << std::endl;
  
  // Delete listeners and unregister them.
  std::cout << "Deleting listeners and unregistering them..." << std::endl;
  delete_listeners(listeners.get(), engine);
  std::cout << "Deleted." << std::endl;
  std::cout << std::endl;

  return 0;
}
