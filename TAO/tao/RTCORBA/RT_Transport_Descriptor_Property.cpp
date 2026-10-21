#include "tao/RTCORBA/RT_Transport_Descriptor_Property.h"

#include "ace/OS_Memory.h"

#if ! defined (__ACE_INLINE__)
#include "tao/RTCORBA/RT_Transport_Descriptor_Property.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/*************************************************************************************************/

TAO_RT_Transport_Descriptor_Property*
TAO_RT_Transport_Descriptor_Private_Connection_Property::duplicate ()
{
  // Construct a copy of our class
  TAO_RT_Transport_Descriptor_Private_Connection_Property *desc_prop = nullptr;

  ACE_NEW_RETURN (desc_prop,
                  TAO_RT_Transport_Descriptor_Private_Connection_Property (this->object_id_),
                  0);

  return desc_prop;
}

CORBA::Boolean
TAO_RT_Transport_Descriptor_Private_Connection_Property::is_equivalent
  (TAO_RT_Transport_Descriptor_Property const *other_prop)
{
  TAO_RT_Transport_Descriptor_Private_Connection_Property const *rhs =
    dynamic_cast<TAO_RT_Transport_Descriptor_Private_Connection_Property const*> (other_prop);
  return (rhs != nullptr &&
          this->object_id_ == rhs->object_id_);
}

/*************************************************************************************************/

TAO_RT_Transport_Descriptor_Property*
TAO_RT_Transport_Descriptor_Banded_Connection_Property::duplicate ()
{
  // Construct a copy of our class
  TAO_RT_Transport_Descriptor_Banded_Connection_Property *desc_prop = nullptr;

  ACE_NEW_RETURN (desc_prop,
                  TAO_RT_Transport_Descriptor_Banded_Connection_Property (this->low_priority_,
                                  this->high_priority_),
                  0);

  return desc_prop;
}

CORBA::Boolean
TAO_RT_Transport_Descriptor_Banded_Connection_Property::is_equivalent
  (TAO_RT_Transport_Descriptor_Property const *other_prop)
{
  TAO_RT_Transport_Descriptor_Banded_Connection_Property const *rhs =
    dynamic_cast<TAO_RT_Transport_Descriptor_Banded_Connection_Property const*> (other_prop);

  return (rhs != nullptr &&
          this->low_priority_ == rhs->low_priority_ &&
          this->high_priority_ == rhs->high_priority_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
