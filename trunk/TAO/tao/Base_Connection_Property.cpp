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
  if (endpt == 0)
    return 0;

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
  TAO_Base_Connection_Property *other_desc =
    ACE_const_cast (TAO_Base_Connection_Property *,
                    rhs);
  
  if (other_desc == 0)
    return 0;
  
  return this->endpoint_->is_equivalent (other_desc->endpoint_);
}


u_long
TAO_Base_Connection_Property::hash (void) const
{
  return this->endpoint_->hash ();
}
