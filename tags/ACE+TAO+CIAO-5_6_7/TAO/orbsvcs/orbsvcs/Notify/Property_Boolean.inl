// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Notify_Property_Boolean&
TAO_Notify_Property_Boolean::operator= (const TAO_Notify_Property_Boolean& rhs)
{
  if (this == &rhs)
    return *this;

  if (rhs.is_valid ())
    {
      this->name_ = rhs.name_;
      this->value_ = rhs.value_;
      this->valid_ = rhs.valid_;
    }

  return *this;
}

ACE_INLINE TAO_Notify_Property_Boolean&
TAO_Notify_Property_Boolean::operator= (const CORBA::Boolean& value)
{
  this->value_ = value;

  return *this;
}

ACE_INLINE bool
TAO_Notify_Property_Boolean::operator== (const CORBA::Boolean &rhs) const
{
  return (this->value_ == rhs);
}

ACE_INLINE bool
TAO_Notify_Property_Boolean::operator!= (const CORBA::Boolean &rhs) const
{
  return (this->value_ != rhs);
}

ACE_INLINE
const char *
TAO_Notify_Property_Boolean::name (void) const
{
  return this->name_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Property_Boolean::value (void) const
{
  return this->value_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Property_Boolean::is_valid (void) const
{
  return this->valid_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
