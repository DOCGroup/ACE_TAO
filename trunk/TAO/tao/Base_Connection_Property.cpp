//$Id$

#include "tao/Base_Connection_Property.h"



#if !defined (__ACE_INLINE__)
# include "tao/Base_Connection_Property.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Base_Connection_Property, "$Id$")


TAO_Base_Connection_Property::~TAO_Base_Connection_Property (void)
{
}


TAO_Connection_Descriptor_Interface *
TAO_Base_Connection_Property::duplicate (void)
{
  // Get a copy of the underlying endpoint
  TAO_Endpoint *endpt = this->endpoint_->duplicate ();

  // Construct a copy of our class
  TAO_Base_Connection_Property *prop = 0;
  ACE_NEW_RETURN (prop,
                  TAO_Base_Connection_Property (endpt,
                                                1),
                  0);
  return prop;
}


CORBA::Boolean
TAO_Base_Connection_Property::is_equivalent (
    const TAO_Connection_Descriptor_Interface *rhs)
{
  TAO_Connection_Descriptor_Interface  *base =
    ACE_const_cast (TAO_Connection_Descriptor_Interface *,
                    rhs);

  if (this->endpoint_->is_equivalent (base->endpoint ()))
    return 1;

  return 0;
}


u_long
TAO_Base_Connection_Property::hash (void) const
{
  return this->endpoint_->hash ();
}
