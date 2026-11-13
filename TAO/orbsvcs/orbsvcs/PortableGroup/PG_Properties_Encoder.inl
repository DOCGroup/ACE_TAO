// -*- C++ -*-

/**
 *  @file    PG_Properties_Encoder.inl
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */

#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_PG
{
  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue (
    ACE_CString const & name,
    PortableGroup::Value const & value)
    : name_ (name)
    , value_ (value)
  {
  }

  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue (Properties_Encoder::NamedValue const & rhs)
    : name_ (rhs.name_)
    , value_ (rhs.value_)
  {
  }

  ACE_INLINE
  Properties_Encoder::NamedValue &
  Properties_Encoder::NamedValue::operator = (Properties_Encoder::NamedValue const & rhs)
  {
    if (this != std::addressof (rhs))
    {
      this->name_ = rhs.name_;
      this->value_ = rhs.value_;
    }
    return *this;
  }

} //namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL
