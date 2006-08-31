#include "logic_rules.hpp"

namespace OCML
{

  /*
   * Not_Rule implementation
   */

  bool
  Not_Rule::test() const
  {
    return !(rule_->test());
  }

  bool
  Not_Rule::apply(bool negate)
  {
    return rule_->apply(!negate);
  }

  /*
   * And_Rule implementation
   */
  And_Rule::And_Rule(const And_Rule& rule)
    : Basic_Rule<And_Rule>(rule.root_rule_)
  {
    for (std::list<Rule*>::const_iterator iter = rule.rules_.begin();
         iter != rule.rules_.end(); ++iter)
    {
      rules_.push_back((*iter)->duplicate());
    }
  }

  And_Rule::~And_Rule()
  {
    for (std::list<Rule*>::iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
    {
      delete *iter;
    }
  }

  bool
  And_Rule::test() const
  {
    for (std::list<Rule*>::const_iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
      if ((*iter)->test() == false)
        return false;
    return true;
  }

  bool
  And_Rule::apply(bool negate)
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
      if ((*iter)->apply(negate) == negate)
        return negate;
    return !negate;
  }

  /*
   * Or_Rule implementation
   */

  Or_Rule::Or_Rule(const Or_Rule& rule)
    : Basic_Rule<Or_Rule>(rule.root_rule_)
  {
    for (std::list<Rule*>::const_iterator iter = rule.rules_.begin();
         iter != rule.rules_.end(); ++iter)
    {
      rules_.push_back((*iter)->duplicate());
    }
  }

  Or_Rule::~Or_Rule()
  {
    for (std::list<Rule*>::iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
    {
      delete *iter;
    }
  }

  bool
  Or_Rule::test() const
  {
    for (std::list<Rule*>::const_iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
      if ((*iter)->test() == true)
        return true;
    return false;
  }

  bool
  Or_Rule::apply(bool negate)
  {
    // For non-negated OR (negate = false)
    //    Checks all of the operands
    //    if any one of them returns true func returns true
    //                               otherwise returns false
    // For negated OR (negate = true)
    //    Checks all of the operands
    //    if any one of them returns false func returns false
    //                               otherwise returns true
    for (std::list<Rule*>::iterator iter = rules_.begin();
         iter != rules_.end(); ++iter)
      if ((*iter)->apply(negate) == !negate)
        return !negate;
    return negate;
  }

  /*
   * If_Rule implementation
   */
  bool
  If_Rule::test() const
  {
    if (condition_->test())
      return action_->test();
    else
      return true;
  }

  bool
  If_Rule::apply(bool negate)
  {
    // For non-negated IF (negate = false)
    // Tests the condition:
    //      if it is true, applies and returns action(false)
    //      otherwise, returns true
    // For negated IF (negate = true)
    // Tests the condition:
    //      if it is false returns false
    //      if it is true, applies and returns action(true)
    if (condition_->test() == false)
      return !negate;
    return action_->apply(negate);
  }

  /*
   * Iff_Rule implementation
   */
  bool
  Iff_Rule::test() const
  {
    if (rules_.first->test())
      return rules_.second->test();
    else
      return !(rules_.second->test());
  }

  bool
  Iff_Rule::apply(bool negate)
  {
    bool result = true;
    if (rules_.first->test() == true)
      result = rules_.second->apply(negate);
    if (!result)
      return false;

    if (rules_.second->test() == true)
      result = rules_.first->apply(negate);

    return result;
  }

} // namespace OCML
