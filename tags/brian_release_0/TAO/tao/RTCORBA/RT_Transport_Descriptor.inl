// $Id$

#include "RT_Transport_Descriptor.h"

ACE_INLINE
TAO_RT_Transport_Descriptor::TAO_RT_Transport_Descriptor (TAO_Endpoint *endpoint,
                                                          CORBA::Boolean flag)
  : TAO_Transport_Descriptor_Interface (endpoint, flag)
  , property_list_ (0)
  , delete_properties_ (flag)
{
}

ACE_INLINE void
TAO_RT_Transport_Descriptor::insert (TAO_RT_Transport_Descriptor_Property *descriptor_property)
{
  descriptor_property->next_ = this->property_list_;

  this->property_list_ = descriptor_property;
}
