#include <engine/option_traits.hpp>
#include <engine/option_reference.hpp>
#include <engine/logic_rules.hpp>
#include <engine/option_rules.hpp>

#include "rules_test.hpp"
#include "definitions_test.hpp"

#include <list>
#include <memory>

struct find_ref_t
{
  find_ref_t(OCML::Option_Category* category,
             references_map* references)
    : category_(category), references_(references)
  {
  }

  OCML::Option_Reference* operator()(const char* option_name)
  {
    OCML::Option_Definition* option = category_->option(option_name);
    return (*references_)[option];
  }

private:
  OCML::Option_Category* category_;
  references_map* references_;
};

int main(int, char*[])
{
  // create category
  std::auto_ptr<OCML::Option_Category>
    category(create_category());
  // create references
  std::auto_ptr<references_map>
    references(create_references(category.get()));
  // create root rule
  OCML::Root_Rule root_rule;

  std::cout << "References initially:\n";
  write_info(references.get());
 
  std::auto_ptr<OCML::Rule> rule;
  bool result;
  find_ref_t find_ref(category.get(), references.get());

  // Fetch basic rules.
  OCML::Boolean_Option_Reference* basic_bool_reference =
    dynamic_cast<OCML::Boolean_Option_Reference*> (find_ref("bool_def_test"));
  OCML::Integer_Option_Reference* basic_int_reference =
    dynamic_cast<OCML::Integer_Option_Reference*> (find_ref("int_def_test"));
  OCML::String_Option_Reference* basic_str_reference =
    dynamic_cast<OCML::String_Option_Reference*> (find_ref("str_def_test"));

  /*
   *  Integer option selection test.
   */
  rule.reset(new OCML::Option_Selection_Rule(&root_rule,
                                           find_ref("int_def_test")));
  OCML::Root_Rule::rule_index int_select_rule = root_rule.add_rule(rule.get());
  std::cout << "Applying (int_option.selected = true)...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << "Integer reference after:" << str_info(find_ref("int_def_test"))
            << std::endl;
  std::cout << "Delete rule.\n";
  root_rule.delete_rule(int_select_rule);
  std::cout << "Applying (delete int_option.selected)...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << "Integer reference after:" << str_info(find_ref("int_def_test"))
            << std::endl;

  /*
   * Integer option equality test.
   */
  rule.reset(
    new OCML::Value_Equality_Rule<OCML::Integer_Option_Trait>(&root_rule,
                                       basic_int_reference,
                                       3));
  OCML::Root_Rule::rule_index int_eq_rule = root_rule.add_rule(rule.get());
  std::cout << "Applying (int_option = 3)...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << str_info(find_ref("int_def_test"))
            << std::endl;
  std::cout << "Delete rule.\n";
  root_rule.delete_rule(int_eq_rule);
  std::cout << "Applying (delete int_option = 3)...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << str_info(find_ref("int_def_test"))
            << std::endl;

  /*
   * If rule test 1.
   */
  std::auto_ptr<OCML::Rule> rule1(
    new OCML::Value_Equality_Rule<OCML::Boolean_Option_Trait>(&root_rule,
                                       basic_bool_reference,
                                       true));
  std::auto_ptr<OCML::Rule> rule2( 
    new OCML::Value_Equality_Rule<OCML::Integer_Option_Trait>(&root_rule,
                                       basic_int_reference,
                                       12));
  rule.reset(new OCML::If_Rule(&root_rule, rule1.get(), rule2.get()));
  OCML::Root_Rule::rule_index if_rule = root_rule.add_rule(rule.get());
  std::cout << "Applying: ((bool_option=true) => (int_option=3))...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << str_info(find_ref("bool_def_test")) << std::endl
            << str_info(find_ref("int_def_test")) << std::endl;
  std::cout << "Delete rule.\n";
  root_rule.delete_rule(if_rule);
  std::cout << "Applying (delete if rule)...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << str_info(find_ref("bool_def_test")) << std::endl
            << str_info(find_ref("int_def_test")) << std::endl;
        
  /*
   * If rule test 2.
   */
  rule1.reset(
    new OCML::Value_Equality_Rule<OCML::Boolean_Option_Trait>(&root_rule,
                                       basic_bool_reference,
                                       true));
  rule2.reset( 
    new OCML::Value_Equality_Rule<OCML::Integer_Option_Trait>(&root_rule,
                                       basic_int_reference,
                                       12));
  rule.reset(new OCML::If_Rule(&root_rule, rule1.get(), rule2.get()));
  OCML::Root_Rule::rule_index if_rule2 = root_rule.add_rule(rule.get());
  std::auto_ptr<OCML::Rule> rule3(
    new OCML::Value_Equality_Rule<OCML::Boolean_Option_Trait>(&root_rule,
                                       basic_bool_reference,
                                       true));
  OCML::Root_Rule::rule_index bool_eq_rule2 = root_rule.add_rule(rule3.get());

  std::cout << "Applying: (bool_option=true) && ((bool_option=true) => (int_option=3))...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << str_info(find_ref("bool_def_test")) << std::endl
            << str_info(find_ref("int_def_test")) << std::endl;
  std::cout << "Delete rule.\n";
  root_rule.delete_rule(if_rule2);
  root_rule.delete_rule(bool_eq_rule2);
  std::cout << "Applying (delete if rule)...";
  clear_references(references.get());
  result = root_rule.apply();
  std::cout << "  result: " << bool_as_string(result) << std::endl;
  std::cout << str_info(find_ref("bool_def_test")) << std::endl
            << str_info(find_ref("int_def_test")) << std::endl;
 
 /* 
  // create rules
  std::auto_ptr<OCML::Root_Rule>
    root_rule(create_rules(category.get(), references));

  std::cout << "Before applying rule:" << std::endl;
  write_info(references);

  bool result = root_rule->apply();
  std::cout << "Constraint ";
  if (result)
    std::cout << " satisfied.\n";
  else
    std::cout << " failed.\n";
  
  std::cout << "After applying rule:" << std::endl;
  write_info(reference_list);
*/
  return 0;
}
