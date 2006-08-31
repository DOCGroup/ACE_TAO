// $Id$

#include <common/exception.hpp>

namespace OCML
{

  template <typename OPTION_TRAIT>
  Basic_Option_Value<OPTION_TRAIT>::
  Basic_Option_Value()
  {
  }
  
  template <typename OPTION_TRAIT>
  Basic_Option_Value<OPTION_TRAIT>::
  Basic_Option_Value(const Basic_Option_Value<option_trait>& value)
    : value_(new value_type(value.value()))
  {
  }

  template <typename OPTION_TRAIT>
  Basic_Option_Value<OPTION_TRAIT>::
  Basic_Option_Value(
      const typename Basic_Option_Value<OPTION_TRAIT>::value_type& value)
    : value_(new value_type(value))
  {
  }

  template <typename OPTION_TRAIT>
  Basic_Option_Value<OPTION_TRAIT>::
  Basic_Option_Value(const char* first, const char* last)
    : value_(new value_type(OPTION_TRAIT::parse_string(first, last)))
  {
  }

  template <typename OPTION_TRAIT>
  Basic_Option_Value<OPTION_TRAIT>::
  ~Basic_Option_Value()
  {
  }

  template <typename OPTION_TRAIT>
  void
  Basic_Option_Value<OPTION_TRAIT>::
  value(const typename Basic_Option_Value<OPTION_TRAIT>::value_type& value)
  {
    value_.reset(new value_type(value));
  }

  template <typename OPTION_TRAIT>
  typename Basic_Option_Value<OPTION_TRAIT>::value_type
  Basic_Option_Value<OPTION_TRAIT>::
  value() const
  {
    if (!value_.get())
    {
      throw Null_Value_Exception();
    }

    return *(value_.get());
  }

  template <typename OPTION_TRAIT>
  bool
  Basic_Option_Value<OPTION_TRAIT>::
  is_null() const
  {
    return value_.get() == 0;
  }

  template <typename OPTION_TRAIT>
  void
  Basic_Option_Value<OPTION_TRAIT>::
  clear()
  {
    value_.release();
  }

  template <typename OPTION_TRAIT>
  void
  Basic_Option_Value<OPTION_TRAIT>::
  read(std::istream& is)
  {
    value_type v;
    v = option_trait::read(is);
    value_.reset(new value_type(v));
  }

  template <typename OPTION_TRAIT>
  void
  Basic_Option_Value<OPTION_TRAIT>::
  write(std::ostream& os) const
  {
    if (value_.get() == 0)
    {
      throw Null_Value_Exception();
    }
    option_trait::write(os, *(value_.get()));
  }

  template <typename OPTION_TRAIT>
  void
  Basic_Option_Value<OPTION_TRAIT>::
  set(const Option_Value* option_value)
  {
    const Basic_Option_Value<option_trait>* basic_option_value =
      dynamic_cast<const Basic_Option_Value<option_trait>*> (option_value);

    if (basic_option_value != 0)
    {
      value_type val = basic_option_value->value();
      value_.reset(new value_type(val));
    }
    else
    {
      throw Value_Type_Mismatch();
    }
  }

  template <typename OPTION_TRAIT>
  bool
  Basic_Option_Value<OPTION_TRAIT>::
  compare(const Option_Value* option_value) const
  {
    const Basic_Option_Value<option_trait>* basic_option_value = 
      dynamic_cast<const Basic_Option_Value*>(option_value);

    if (basic_option_value != 0)
    {
      // if any of the values is null then result is false.
      if (basic_option_value->is_null() || this->is_null())
        return false;

     return basic_option_value->value() == value();
    }
    else
    {
      throw Value_Type_Mismatch();
    }
  }

  template <typename OPTION_TRAIT>
  Option_Value*
  Basic_Option_Value<OPTION_TRAIT>::
  duplicate() const
  {
    Basic_Option_Value<OPTION_TRAIT>* value =
      new Basic_Option_Value<OPTION_TRAIT>;

    if (value_.get() != 0)
      value->value_.reset(new value_type(*value_));

    return value;
  }

} // namespace OCML
