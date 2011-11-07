// $Id$

#include "tao/RTCORBA/RT_Transport_Descriptor.h"
#include "ace/OS_Memory.h"

#if ! defined (__ACE_INLINE__)
#include "tao/RTCORBA/RT_Transport_Descriptor.inl"
#endif /* __ACE_INLINE__ */

#include "tao/RTCORBA/RT_Transport_Descriptor_Property.h"
#include "tao/Endpoint.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Transport_Descriptor::~TAO_RT_Transport_Descriptor ()
{
  if (this->delete_properties_ == 1)
    {
      TAO_RT_Transport_Descriptor_Property *current =
        this->property_list_;

      while (current)
        {
          TAO_RT_Transport_Descriptor_Property *next =
            current->next_;

          delete current;

          current = next;
        }
    }
}

TAO_Transport_Descriptor_Interface *
TAO_RT_Transport_Descriptor::duplicate (void)
{
  // Get a copy of the underlying endpoint
  TAO_Endpoint *endpoint =
    this->endpoint_->duplicate ();
  if (endpoint == 0)
    return 0;

  TAO_RT_Transport_Descriptor *new_descriptor = 0;

  ACE_NEW_RETURN (new_descriptor,
                  TAO_RT_Transport_Descriptor (endpoint, 1),
                  0);

  // Copy the Properties.
  TAO_RT_Transport_Descriptor_Property *current_property =
    this->property_list_;

  TAO_RT_Transport_Descriptor_Property *current_new_property = 0;
  TAO_RT_Transport_Descriptor_Property *new_property = 0;

  while (current_property)
    {
      new_property =
        current_property->duplicate ();

      // Note that we cannot use <insert> because that will reverse the stack.
      if (new_descriptor->property_list_ == 0)
        new_descriptor->property_list_ = new_property;
      else if (current_new_property != 0)
        current_new_property->next_ = new_property;

      current_new_property = new_property;
      current_property = current_property->next_;
    }

  return new_descriptor;
}

CORBA::Boolean
TAO_RT_Transport_Descriptor::is_equivalent (const TAO_Transport_Descriptor_Interface *other_prop)
{
  const TAO_RT_Transport_Descriptor *rhs =
    dynamic_cast<const TAO_RT_Transport_Descriptor*> (other_prop);

  if (rhs == 0)
    return false;

  // Check if endpoint is equivalent.
  if (this->endpoint_->is_equivalent (rhs->endpoint_) == 0)
    return false;

  // Check the property_list_.
  TAO_RT_Transport_Descriptor_Property *current =
    this->property_list_;

  TAO_RT_Transport_Descriptor_Property *rhs_current =
    rhs->property_list_;

  while (current || rhs_current)
    {
      if (rhs_current == 0 || current == 0)
        return false;

      if (current->is_equivalent (rhs_current) == 0)
        return false;

      current = current->next_;
      rhs_current = rhs_current->next_;
    }

  return true;
}

u_long
TAO_RT_Transport_Descriptor::hash (void) const
{
  return this->endpoint_->hash ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
