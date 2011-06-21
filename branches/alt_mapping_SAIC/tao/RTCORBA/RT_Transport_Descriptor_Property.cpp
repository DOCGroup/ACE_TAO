// $Id$

#include "tao/RTCORBA/RT_Transport_Descriptor_Property.h"

#include "ace/OS_Memory.h"

#if ! defined (__ACE_INLINE__)
#include "tao/RTCORBA/RT_Transport_Descriptor_Property.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Transport_Descriptor_Property::~TAO_RT_Transport_Descriptor_Property (void)
{
}

/*************************************************************************************************/

TAO_RT_Transport_Descriptor_Private_Connection_Property::~TAO_RT_Transport_Descriptor_Private_Connection_Property (void)
{
}

TAO_RT_Transport_Descriptor_Property*
TAO_RT_Transport_Descriptor_Private_Connection_Property::duplicate (void)
{
  // Construct a copy of our class
  TAO_RT_Transport_Descriptor_Private_Connection_Property *desc_prop = 0;

  ACE_NEW_RETURN (desc_prop,
                  TAO_RT_Transport_Descriptor_Private_Connection_Property (this->object_id_),
                  0);

  return desc_prop;
}

CORBA::Boolean
TAO_RT_Transport_Descriptor_Private_Connection_Property::is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop)
{
  const TAO_RT_Transport_Descriptor_Private_Connection_Property *rhs =
    dynamic_cast<const TAO_RT_Transport_Descriptor_Private_Connection_Property*> (other_prop);
  return (rhs != 0 &&
          this->object_id_ == rhs->object_id_);
}

/*************************************************************************************************/

TAO_RT_Transport_Descriptor_Banded_Connection_Property::~TAO_RT_Transport_Descriptor_Banded_Connection_Property ()
{
}

TAO_RT_Transport_Descriptor_Property*
TAO_RT_Transport_Descriptor_Banded_Connection_Property::duplicate (void)
{
  // Construct a copy of our class
  TAO_RT_Transport_Descriptor_Banded_Connection_Property *desc_prop = 0;

  ACE_NEW_RETURN (desc_prop,
                  TAO_RT_Transport_Descriptor_Banded_Connection_Property (this->low_priority_,
                                  this->high_priority_),
                  0);

  return desc_prop;
}

CORBA::Boolean
TAO_RT_Transport_Descriptor_Banded_Connection_Property::is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop)
{
  const TAO_RT_Transport_Descriptor_Banded_Connection_Property *rhs =
    dynamic_cast<const TAO_RT_Transport_Descriptor_Banded_Connection_Property*> (other_prop);

  return (rhs != 0 &&
          this->low_priority_ == rhs->low_priority_ &&
          this->high_priority_ == rhs->high_priority_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
