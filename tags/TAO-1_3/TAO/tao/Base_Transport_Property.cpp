//$Id$

#include "tao/Base_Transport_Property.h"



#if !defined (__ACE_INLINE__)
# include "tao/Base_Transport_Property.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Base_Transport_Property, "$Id$")


TAO_Base_Transport_Property::~TAO_Base_Transport_Property (void)
{
}


TAO_Transport_Descriptor_Interface *
TAO_Base_Transport_Property::duplicate (void)
{
  // Get a copy of the underlying endpoint
  TAO_Endpoint *endpt = this->endpoint_->duplicate ();
  if (endpt == 0)
    return 0;

  // Construct a copy of our class
  TAO_Base_Transport_Property *prop = 0;
  ACE_NEW_RETURN (prop,
                  TAO_Base_Transport_Property (endpt,
                                               1),
                  0);
  return prop;
}


CORBA::Boolean
TAO_Base_Transport_Property::is_equivalent (
    const TAO_Transport_Descriptor_Interface *rhs)
{
  // Do away with const.
  TAO_Transport_Descriptor_Interface *r =
    ACE_const_cast (TAO_Transport_Descriptor_Interface *,
                    rhs);

  TAO_Base_Transport_Property *other_desc =
    ACE_dynamic_cast (TAO_Base_Transport_Property *,
                      r);

  if (other_desc == 0)
    return 0;

  return this->endpoint_->is_equivalent (other_desc->endpoint_);
}


u_long
TAO_Base_Transport_Property::hash (void) const
{
  return this->endpoint_->hash ();
}
