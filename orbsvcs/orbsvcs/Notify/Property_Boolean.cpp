// $Id$

#include "orbsvcs/Notify/Property_Boolean.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Property_Boolean.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/PropertySeq.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Property_Boolean::TAO_Notify_Property_Boolean (const char* name)
  : name_ (name), valid_(0)
{
}

TAO_Notify_Property_Boolean::TAO_Notify_Property_Boolean (const char* name, CORBA::Boolean initial)
  :name_ (name), value_ (initial), valid_ (1)
{
}

int
TAO_Notify_Property_Boolean::set (const TAO_Notify_PropertySeq& property_seq)
{
  CosNotification::PropertyValue value;

  if (property_seq.find (this->name_, value) == -1)
    return -1;

  value >>= CORBA::Any::to_boolean (this->value_);

  return 0;
}

void
TAO_Notify_Property_Boolean::get (CosNotification::PropertySeq& prop_seq)
{
  /// Make space
  prop_seq.length (prop_seq.length () + 1);

  prop_seq[prop_seq.length () - 1].value <<= CORBA::Any::from_boolean (this->value_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
