namespace OCML
{
    
  inline
  Option_Selection_Rule::Option_Selection_Rule(Rule* root,
                                           Option_Reference* option)
    : Basic_Rule<Option_Selection_Rule>(root),
      option_(option)
  {
  }

  inline bool
  Option_Selection_Rule::test() const
  {
    return option_->assigned();
  }

  inline bool
  Option_Selection_Rule::apply(bool negate)
  {
    if (negate)
      return make_not_select();
    else
      return make_select();
  }

  inline bool
  Option_Selection_Rule::make_select()
  {
    // If already required return true
    if (option_->required())
      return true;

    // If not allowed cannot be required
    if (!option_->allowed())
      return false;

    // Try to set required
    option_->required(true);
    if (check_validity())
      return true;
    option_->required(false);
    return false;
  }

  inline bool
  Option_Selection_Rule::make_not_select()
  {
    // If already not allowed return true
    if (!option_->allowed())
      return true;

    // If required must be allowed
    if (option_->required())
      return false;

    // If already assigned to a value must be allowed
    if (option_->assigned())
      return false;

    // Try to set allowed to false
    option_->allowed(false);

    if (check_validity())
      return true;
    option_->allowed(true);
    return false;
  }

} // namespace OCML
