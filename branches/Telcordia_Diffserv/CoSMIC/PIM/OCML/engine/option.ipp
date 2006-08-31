// $Id$

#include "constraint_engine.hpp"

namespace OCML
{
  inline
  Option::Option(Option_Definition* option_definition,
                 Constraint_Engine* constraint_manager)
    : definition_(option_definition),
      constraint_manager_(constraint_manager),
      assigned_(false)
  {
  }

  inline bool
  Option::clear()
  {
    if (!assigned_)
    {
      return true;
    }

    constraint_manager_->delete_rule(equality_rule_);

    // If validated no more assigned, otherwise still assigned.
    assigned_ = !(constraint_manager_->validate());

    return !assigned_;
  }

  inline bool
  Option::value(const Option_Value* value)
  {
    bool initially_assigned = assigned_;

    Constraint_Engine::rule_index tmp_equality_rule = equality_rule_;
   
    if (assigned_)
    {
      constraint_manager_->delete_rule(equality_rule_);
    }

    equality_rule_ =
      constraint_manager_->define_equality_rule(definition_, value);

    constraint_manager_->add_rule(equality_rule_);

    // If validated now assigned, otherwise not assigned.
    assigned_ = constraint_manager_->validate();

    if (!assigned_)
    {
      constraint_manager_->delete_rule_definition(equality_rule_);
      if (initially_assigned)
      {
        equality_rule_ = tmp_equality_rule;
        constraint_manager_->add_rule(equality_rule_);
        constraint_manager_->validate();
      }
    }

    return assigned_;
  }

} // namespace OCML
