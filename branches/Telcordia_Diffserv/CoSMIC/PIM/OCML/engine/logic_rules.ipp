// $Id$

namespace OCML
{

  inline
  Not_Rule::Not_Rule(const Not_Rule& rule)
    : Basic_Rule<Not_Rule>(rule.root_rule_),
      rule_(rule.rule_->duplicate())
  {
  }
  
  inline
  Not_Rule::Not_Rule(Rule* root, Rule* rule)
    : Basic_Rule<Not_Rule>(root), rule_(rule)
  {
  }

  inline
  If_Rule::If_Rule(const If_Rule& rule)
    : Basic_Rule<If_Rule>(rule.root_rule_),
      condition_(rule.condition_->duplicate()),
      action_(rule.action_->duplicate())
  {
  }
  
  inline
  If_Rule::If_Rule(Rule* root, Rule* condition, Rule* action)
    : Basic_Rule<If_Rule>(root),
      condition_(condition), action_(action)
  {
  }

  inline
  Iff_Rule::Iff_Rule(const Iff_Rule& rule)
    : Basic_Rule<Iff_Rule>(rule.root_rule_)
  {
    rules_.first.reset(rule.rules_.first->duplicate());
    rules_.second.reset(rule.rules_.second->duplicate());
  }

  inline
  Iff_Rule::Iff_Rule(Rule* root, Rule* rule1, Rule* rule2)
    : Basic_Rule<Iff_Rule>(root)
  {
    rules_.first.reset(rule1);
    rules_.second.reset(rule2);
  }

} // namespace OCML
