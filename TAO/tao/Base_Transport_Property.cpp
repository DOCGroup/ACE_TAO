#include "tao/Base_Transport_Property.h"
#include "ace/OS_Memory.h"

#if !defined (__ACE_INLINE__)
# include "tao/Base_Transport_Property.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Base_Transport_Property::~TAO_Base_Transport_Property ()
{
}

TAO_Transport_Descriptor_Interface *
TAO_Base_Transport_Property::duplicate ()
{
  // Get a copy of the underlying endpoint
  TAO_Endpoint *const endpt = this->endpoint_->duplicate ();
  if (endpt == nullptr)
    return nullptr;

  // Construct a copy of our class
  TAO_Base_Transport_Property *prop = nullptr;
  ACE_NEW_RETURN (prop, TAO_Base_Transport_Property (endpt, true), nullptr);
  return prop;
}


CORBA::Boolean
TAO_Base_Transport_Property::is_equivalent
  (const TAO_Transport_Descriptor_Interface *rhs)
{
  const TAO_Base_Transport_Property *other_desc =
    dynamic_cast<const TAO_Base_Transport_Property *> (rhs);

  if (other_desc == nullptr)
    return false;

  return this->endpoint_->is_equivalent (other_desc->endpoint_);
}


u_long
TAO_Base_Transport_Property::hash () const
{
  return this->endpoint_->hash ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
