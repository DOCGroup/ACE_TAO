#include "Private_Transport_Descriptor.h"

#if !defined (__ACE_INLINE__)
# include "Private_Transport_Descriptor.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Endpoint.h"

ACE_RCSID (tao,
           Private_Transport_Descriptor,
           "$Id$")


TAO_Private_Transport_Descriptor::~TAO_Private_Transport_Descriptor (void)
{
}


TAO_Transport_Descriptor_Interface *
TAO_Private_Transport_Descriptor::duplicate (void)
{
  // Get a copy of the underlying endpoint.
  TAO_Endpoint *endp = this->endpoint_->duplicate ();
  if (endp == 0)
    return 0;

  // Construct a copy of our class
  TAO_Private_Transport_Descriptor *desc = 0;
  ACE_NEW_RETURN (desc,
                  TAO_Private_Transport_Descriptor (endp,
                                                     this->object_id_,
                                                     1),
                  0);
  return desc;
}


CORBA::Boolean
TAO_Private_Transport_Descriptor::is_equivalent (
    const TAO_Transport_Descriptor_Interface *rhs)
{
  // Do away with const first.
  TAO_Transport_Descriptor_Interface *r =
    ACE_const_cast (TAO_Transport_Descriptor_Interface *,
                    rhs);

  TAO_Private_Transport_Descriptor *other_desc =
    ACE_dynamic_cast (TAO_Private_Transport_Descriptor *,
                      r);

  if (other_desc == 0)
    return 0;

  return (this->endpoint_->is_equivalent (other_desc->endpoint_)
          && this->object_id_ == other_desc->object_id_);
}


u_long
TAO_Private_Transport_Descriptor::hash (void) const
{
  return (this->endpoint_->hash () + this->object_id_);
}

