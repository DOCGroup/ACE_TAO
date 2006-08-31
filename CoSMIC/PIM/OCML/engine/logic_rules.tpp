// $Id$

namespace OCML
{
    
  template <typename InputIterator>
  And_Rule::And_Rule(Rule* root, InputIterator first, InputIterator last)
    : Basic_Rule<And_Rule>(root),
      rules_(first, last)
  {
  }

  template <typename InputIterator>
  Or_Rule::Or_Rule(Rule* root, InputIterator first, InputIterator last)
    : Basic_Rule<Or_Rule>(root),
      rules_(first, last)
  {
  }

} // namespace OCML
