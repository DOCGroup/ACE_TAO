//$Id$

#include "tao/Base_Connection_Property.h"



#if !defined (__ACE_INLINE__)
# include "tao/Base_Connection_Property.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Base_Connection_Property, "$Id$")


TAO_Base_Connection_Property::~TAO_Base_Connection_Property (void)
{
  if (endpoint_flag_)
    delete this->endpoint_;
}


TAO_Base_Connection_Property *
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
