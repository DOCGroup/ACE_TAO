// $Id$

#include "RT_Transport_Descriptor_Property.h"

ACE_INLINE
TAO_RT_Transport_Descriptor_Property::TAO_RT_Transport_Descriptor_Property (void)
  : next_ (0)
{
}

/*************************************************************************************************/
ACE_INLINE
TAO_RT_Transport_Descriptor_Private_Connection_Property::TAO_RT_Transport_Descriptor_Private_Connection_Property (void)
  : object_id_ (-1)
{
}

ACE_INLINE
TAO_RT_Transport_Descriptor_Private_Connection_Property::TAO_RT_Transport_Descriptor_Private_Connection_Property (long object_id)
  : object_id_ (object_id)
{
}

ACE_INLINE void
TAO_RT_Transport_Descriptor_Private_Connection_Property::init (long object_id)
{
  this->object_id_ = object_id;
}

/*************************************************************************************************/

ACE_INLINE
TAO_RT_Transport_Descriptor_Banded_Connection_Property::TAO_RT_Transport_Descriptor_Banded_Connection_Property (void)
  : low_priority_ (-1)
  , high_priority_ (-1)
{
}

ACE_INLINE
TAO_RT_Transport_Descriptor_Banded_Connection_Property::TAO_RT_Transport_Descriptor_Banded_Connection_Property (CORBA::Short low_priority,
                                                                                                                CORBA::Short high_priority)
  : low_priority_ (low_priority)
  , high_priority_ (high_priority)
{
}

ACE_INLINE void
TAO_RT_Transport_Descriptor_Banded_Connection_Property::init (CORBA::Short low_priority,
                                                              CORBA::Short high_priority)
{
  this->low_priority_ = low_priority;
  this->high_priority_ = high_priority;
}
