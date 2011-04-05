//$Id$

#include "tao/Base_Transport_Property.h"
#include "ace/OS_Memory.h"

#if !defined (__ACE_INLINE__)
# include "tao/Base_Transport_Property.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Base_Transport_Property::~TAO_Base_Transport_Property (void)
{
}

TAO_Transport_Descriptor_Interface *
TAO_Base_Transport_Property::duplicate (void)
{
  // Get a copy of the underlying endpoint
  TAO_Endpoint *const endpt = this->endpoint_->duplicate ();
  if (endpt == 0)
    return 0;

  // Construct a copy of our class
  TAO_Base_Transport_Property *prop = 0;
  ACE_NEW_RETURN (prop, TAO_Base_Transport_Property (endpt, true), 0);
  return prop;
}


CORBA::Boolean
TAO_Base_Transport_Property::is_equivalent
  (const TAO_Transport_Descriptor_Interface *rhs)
{
  const TAO_Base_Transport_Property *other_desc =
    dynamic_cast<const TAO_Base_Transport_Property *> (rhs);

  if (other_desc == 0)
    return false;

  return this->endpoint_->is_equivalent (other_desc->endpoint_);
}


u_long
TAO_Base_Transport_Property::hash (void) const
{
  return this->endpoint_->hash ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
