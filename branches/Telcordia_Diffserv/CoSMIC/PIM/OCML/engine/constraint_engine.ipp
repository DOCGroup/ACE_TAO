// $Id$

#include "option_rules.hpp"
#include "logic_rules.hpp"

#include <algorithm>

namespace OCML
{

  /*
   * Value changle listener inline methods.
   */

  inline Constraint_Value_Change_Listener::Constraint_Value_Change_Listener()
    : flag_(false)
  {
  }
  
  /*
   * Constraint engine inline methods.
   */

  inline void
  Constraint_Engine::register_listener(Option_Definition* option,
                                       Constraint_Value_Change_Listener* l)
  {
    listeners_[option] = l;
  }

  inline void
  Constraint_Engine::unregister_listener(Option_Definition* option)
  {
    listener_iterator iter = listeners_.find(option);

    if (iter != listeners_.end())
    {
      listeners_.erase(iter);
    }
  }

  inline Constraint_Engine::rule_index
  Constraint_Engine::create_interface(Rule* rule)
  {
    rule_interface* interface = new rule_interface(root_rule_, rule);
    interfaces_.push_front(interface);
    return interfaces_.begin();
  }
  
  inline Constraint_Engine::rule_index
  Constraint_Engine::define_selection_rule(const Option_Definition* option)
  {
    Rule* rule = new Option_Selection_Rule(&root_rule_, references_[option]);
    return create_interface(rule);
  }

  template <typename OPTION_TRAIT>
  Rule*
  Constraint_Engine::
  try_create_equality_rule(const Option_Definition* option,
                           const Option_Value* value)
  {
    typedef Basic_Option_Value<OPTION_TRAIT> option_value_type;
    typedef Basic_Option_Reference<OPTION_TRAIT> reference_type;
    
    Rule* rule = 0;
    const option_value_type* basic_value =
      dynamic_cast<const option_value_type*>(value);

    if (basic_value)
    {
      reference_type* ref =
        dynamic_cast<reference_type*> (references_[option]);
      rule =
        new Value_Equality_Rule<OPTION_TRAIT>(&root_rule_, ref,
                                                basic_value->value());
    }

    return rule;
  }
  
  inline Constraint_Engine::rule_index
  Constraint_Engine::define_equality_rule(const Option_Definition* option,
                                          const Option_Value* value)
  {
    Rule* rule = 0;
 
    if (rule == 0)
      rule=try_create_equality_rule<Boolean_Option_Trait>(option, value);
    if (rule == 0)
      rule=try_create_equality_rule<Integer_Option_Trait>(option, value);
    if (rule == 0)
      rule=try_create_equality_rule<String_Option_Trait>(option, value);
 
    return create_interface(rule);
  }

  inline Constraint_Engine::rule_index
  Constraint_Engine::define_not_rule(rule_index index)
  {
    Rule* rule = new Not_Rule(&root_rule_, (*index)->rule());

    return create_interface(rule);
  }

  inline Constraint_Engine::rule_index
  Constraint_Engine::define_if_rule(rule_index index1, rule_index index2)
  {
    Rule* rule = new If_Rule(&root_rule_, (*index1)->rule(),
                             (*index2)->rule());

    return create_interface(rule);
  }

  inline Constraint_Engine::rule_index
  Constraint_Engine::define_iff_rule(rule_index index1, rule_index index2)
  {
    Rule* rule = new Iff_Rule(&root_rule_, (*index1)->rule(),
                              (*index2)->rule());

    return create_interface(rule);
  }

  inline Constraint_Engine::rule_index
  Constraint_Engine::define_and_rule(const std::vector<rule_index>& indexes)
  {
    std::vector<Rule*> rules(indexes.size());
    for (std::vector<rule_index>::const_iterator iter = indexes.begin();
         iter != indexes.end(); ++iter)
    {
      rules.push_back((**iter)->rule());
    }
    Rule* rule = new And_Rule(&root_rule_, rules.begin(), rules.end());

    return create_interface(rule);
  }

  inline Constraint_Engine::rule_index
  Constraint_Engine::define_or_rule(const std::vector<rule_index>& indexes)
  {
    std::vector<Rule*> rules(indexes.size());
    for (std::vector<rule_index>::const_iterator iter = indexes.begin();
         iter != indexes.end(); ++iter)
    {
      rules.push_back((**iter)->rule());
    }
    Rule* rule = new Or_Rule(&root_rule_, rules.begin(), rules.end());

    return create_interface(rule);
  }

  inline void
  Constraint_Engine::add_rule(rule_index index)
  {
    inserted_rules_.push_back(index);
  }

  inline void
  Constraint_Engine::delete_rule(rule_index index)
  {
    std::list<rule_index>::iterator iter =
      std::find(deleted_rules_.begin(), deleted_rules_.end(), index);

    if (iter == deleted_rules_.end())
    {
      deleted_rules_.push_back(index);
    }
  }

  /*
   * Constraint_Engone::rule_interface inline methods.
   */
  inline
  Constraint_Engine::rule_interface::
  rule_interface(Root_Rule& root_rule, Rule* rule)
    : root_rule_(root_rule), rule_(rule)
  {
  }

  inline void
  Constraint_Engine::rule_interface::
  insert_into_root()
  {
    root_rule_index_ = root_rule_.add_rule(rule_.release());
  }

  inline void
  Constraint_Engine::rule_interface::
  delete_from_root()
  {
    rule_.reset(root_rule_.get_rule(root_rule_index_));
   
    root_rule_.delete_rule(root_rule_index_);
  }

} //namespace OCML
