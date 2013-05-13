// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Properties_Encoder.inl
 *
 *  $Id$
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_PG
{
  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue ()
  {
  }

  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue (const ACE_CString & name, const PortableGroup::Value & value)
    : name_ (name)
    , value_ (value)
  {
  }

  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue (const Properties_Encoder::NamedValue & rhs)
    : name_ (rhs.name_)
    , value_ (rhs.value_)
  {
  }

  ACE_INLINE
  Properties_Encoder::NamedValue &
  Properties_Encoder::NamedValue::operator = (const Properties_Encoder::NamedValue & rhs)
  {
    if (this != &rhs)
    {
      this->name_ = rhs.name_;
      this->value_ = rhs.value_;
    }
    return *this;
  }

  ACE_INLINE
  bool
  Properties_Encoder::NamedValue::operator == (const Properties_Encoder::NamedValue &rhs) const
  {
    return this->name_ == rhs.name_;
  }

  ACE_INLINE
  bool
  Properties_Encoder::NamedValue::operator != (const Properties_Encoder::NamedValue &rhs) const
  {
    return this->name_ != rhs.name_;
  }

} //namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL
