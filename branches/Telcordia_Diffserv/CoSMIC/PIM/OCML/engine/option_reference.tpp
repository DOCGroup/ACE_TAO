#include <algorithm>

namespace OCML
{

  template <typename OPTION_TRAIT> void
  Basic_Option_Reference<OPTION_TRAIT>::value(value_type val)
  {
    value_ = val;
    assigned(true);
  }

  template <typename OPTION_TRAIT>
  typename Basic_Option_Reference<OPTION_TRAIT>::value_type
  Basic_Option_Reference<OPTION_TRAIT>::value() const
  {
    return value_;
  }

  template <typename OPTION_TRAIT> void
  Basic_Option_Reference<OPTION_TRAIT>::clear()
  {
    assigned(false);
  }

  template <typename OPTION_TRAIT>
  Basic_Option_Reference<OPTION_TRAIT>::~Basic_Option_Reference()
  {
  }

  template <typename OPTION_TRAIT> bool
  Basic_Option_Reference<OPTION_TRAIT>::value_banned(value_type value)
  {
    typename std::list<value_type>::iterator iter =
      std::find(banned_values_.begin(), banned_values_.end(), value);

    return (iter != banned_values_.end());
  }

  template <typename OPTION_TRAIT> void
  Basic_Option_Reference<OPTION_TRAIT>::ban_value(value_type value)
  {
    if (!value_banned(value))
      {
        banned_values_.push_back(value);
      }
  }

  template <typename OPTION_TRAIT> void
  Basic_Option_Reference<OPTION_TRAIT>::allow_value(value_type value)
  {
    typename std::list<value_type>::iterator iter =
      std::find(banned_values_.begin(), banned_values_.end(), value);

    if (iter != banned_values_.end())
      {
        banned_values_.erase(iter);
      }
  }

  template <typename OPTION_TRAIT> Option_Value*
  Basic_Option_Reference<OPTION_TRAIT>::option_value() const
  {
    Basic_Option_Value<option_trait>* option_value =
      new Basic_Option_Value<option_trait>();

    if (assigned())
    {
      option_value->value(value());
    }

    return (Option_Value*)option_value;
  }

  template <typename OPTION_TRAIT>
  Basic_Option_Reference<OPTION_TRAIT>*
  Option_Reference_Factory::create_basic_option_reference()
  {
    return new Basic_Option_Reference<OPTION_TRAIT>;
  }

  template <typename OPTION_TRAIT>
  Option_Reference*
  Option_Reference_Factory::try_create(Option_Definition* defn)
  {
    Option_Reference* result = 0;

    if (dynamic_cast<Basic_Option_Definition<OPTION_TRAIT>*>(defn))
      result = create_basic_option_reference<OPTION_TRAIT>();

    return result;
  }
 
} // namespace OCML
