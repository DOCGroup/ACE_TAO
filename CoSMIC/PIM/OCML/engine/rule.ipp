// $Id$

namespace OCML
{

  inline Root_Rule::rule_index
  Root_Rule::add_rule(Rule* rule)
  {
    rules_.push_front(rule);
    return rules_.begin();
  }

  inline void
  Root_Rule::delete_rule(Root_Rule::rule_index index)
  {
    rules_.erase(index);
  }

  inline Rule*
  Root_Rule::get_rule(Root_Rule::rule_index index)
  {
    return *index;
  }

}; // namespace OCML
