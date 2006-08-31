// $Id$

namespace OCML
{

  /*
   * Implementation for the template class Value Equality Rule.
   */

  template <typename OPTION_TRAIT>
  Value_Equality_Rule<OPTION_TRAIT>::
  Value_Equality_Rule(Rule* root,
                    Basic_Option_Reference<option_trait>* option,
                    value_type value)
    : Basic_Rule<Value_Equality_Rule>(root),
      option_(option),
      value_(value)
  {
  }

  template <typename OPTION_TRAIT>
  Value_Equality_Rule<OPTION_TRAIT>::
  ~Value_Equality_Rule()
  {
  }

  template <typename OPTION_TRAIT>
  bool Value_Equality_Rule<OPTION_TRAIT>::test() const
  {
    // if the option is already assigned check if the assigned value
    // is equal to the required value. Otherwise return false.
    if (option_->assigned())
      return value_ == option_->value();
    return false;
  }

  template <typename OPTION_TRAIT>
  bool Value_Equality_Rule<OPTION_TRAIT>::apply(bool negate)
  {
    if (!negate)
      return make_equal();
    else
      return make_not_equal();
  }

  /// (not negate) Tries to set the assigned flag and set option value to
  /// the given value.
  template <typename OPTION_TRAIT>
  bool Value_Equality_Rule<OPTION_TRAIT>::make_equal()
  {
    // If there is a value already assigned check if it is the value
    if (option_->assigned())
      return (option_->value() == value_);

    // If the option is banned return false
    if (option_->value_banned(value_))
      return false;

    // Set the option value & assigned flag.
    option_->value(value_);
    option_->assigned(true);

    if (check_validity())
      return true;
    option_->assigned(false);
    return false;
  }

  /// (negate) Tries to add the value into the ban list.
  template <typename OPTION_TRAIT>
  bool Value_Equality_Rule<OPTION_TRAIT>::make_not_equal()
  {
    // If there is a value already assigned check if it is the value
    if (option_->assigned())
      return (option_->value() != value_);
      
    // If the option is already banned return true
    if (option_->value_banned(value_))
      return true;
      
    // Try to add value into ban list, if fails rollback
    option_->ban_value(value_);

    if (check_validity())
      return true;
    option_->allow_value(value_);
    return false;
  }

  /*
   * Implementation for the template class Option Equality Rule.
   */

  template <typename OPTION_TRAIT>
  Option_Equality_Rule<OPTION_TRAIT>::
  Option_Equality_Rule(Basic_Option_Reference<option_trait>* option1,
                     Basic_Option_Reference<option_trait>* option2)
    : option1_(option1),
      option2_(option2)
  {
  }

  /**
   * This logic assumes null = null.
   */
  template <typename OPTION_TRAIT> bool
  Option_Equality_Rule<OPTION_TRAIT>::
  test() const
  {
    return
      (option1_->assigned() && option2_->assigned() &&
       (option1_->value() == option2_->value())) ||
      (!option1_->assigned() && !option2_->assigned());
  }

  template <typename OPTION_TRAIT> bool
  Option_Equality_Rule<OPTION_TRAIT>::
  apply(bool negate)
  {
    if (!negate)
      return make_equal();
    else
      return make_not_equal();
  }

  template <typename OPTION_TRAIT> bool
  Option_Equality_Rule<OPTION_TRAIT>::
  equate(Basic_Option_Reference<option_trait>* rvalue,
         Basic_Option_Reference<option_trait>* lvalue)
  {
    if (lvalue->assigned())
      {
        return (rvalue->value() == lvalue->value());
      }
    else
      {
        if (lvalue->value_banned(rvalue->value()))
          return false;
        
        lvalue->value(rvalue->value());
        lvalue->assigned(true);
        
        if (check_validity())
          return true;
        
        lvalue->assigned(false);
        return false;
      }
  }

  template <typename OPTION_TRAIT> bool
  Option_Equality_Rule<OPTION_TRAIT>::
  unequate(Basic_Option_Reference<option_trait>* rvalue,
           Basic_Option_Reference<option_trait>* lvalue)
  {
    if (lvalue->assigned())
      {
        return rvalue->value() != lvalue->value();
      }
    
    if (lvalue->value_banned(rvalue->value()))
      return true;
    
    lvalue->ban_value(rvalue->value());
    
    if (check_validity())
      return true;
    
    lvalue->allow_value(rvalue->value());
    return false;
  }

  template <typename OPTION_TRAIT> bool
  Option_Equality_Rule<OPTION_TRAIT>::
  make_equal()
  {
    if (option1_->assigned())
      {
        return equate(option1_, option2_);
      }

    if (option2_->assigned())
      {
        return equate(option2_, option1_);
      }

    return true;
  }

  template <typename OPTION_TRAIT> bool
  Option_Equality_Rule<OPTION_TRAIT>::
  make_not_equal()
  {
    if (option1_->assigned())
      {
        return unequate(option1_, option2_);
      }
    if (option2_->assigned())
      {
        return unequate(option2_, option1_);
      }
    
    return true;
  }

} // namespace OCML
