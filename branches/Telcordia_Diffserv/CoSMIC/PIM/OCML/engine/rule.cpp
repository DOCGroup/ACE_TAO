#include "rule.hpp"

namespace OCML
{

  Rule::Rule(Rule* root)
    : root_rule_(root)
  {
  }

  bool
  Rule::check_validity()
  {
    return root_rule_->apply(false);
  }

  Root_Rule::~Root_Rule()
  {
    for (std::list<Rule*>::iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
      {
        delete (*iter);
      }
  }

  bool Root_Rule::test() const
  {
    for (std::list<Rule*>::const_iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
      {
        if ((*iter)->test() == false)
          {
            return false;
          }
      }
    return true;
  }

  bool Root_Rule::apply(bool negate)
  {
    // For non-negated AND (negate = false)
    //    Checks all of the operands
    //    if any one of them returns false func returns false
    //                               otherwise returns true
    // For negated AND (negate = true)
    //    Checks all of the operands
    //    if any one of them returns true func returns true
    //                               otherwise returns false
    for (std::list<Rule*>::iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
      {
        if ((*iter)->apply(negate) == negate)
          {
            return negate;
          }
      }
    return !negate;
  }

} // namespace OCML
