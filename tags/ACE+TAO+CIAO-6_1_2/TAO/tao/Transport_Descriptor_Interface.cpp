// $Id$

#include "tao/Transport_Descriptor_Interface.h"

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Descriptor_Interface.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Endpoint.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Transport_Descriptor_Interface::~TAO_Transport_Descriptor_Interface (void)
{
  if (this->release_)
    {
      delete this->endpoint_;
    }
}

CORBA::Boolean
TAO_Transport_Descriptor_Interface::reset_endpoint (TAO_Endpoint *ep)
{
  // calling on a dynamically allocation descriptor is not allowed.
  if (this->release_)
    return false;

  for (TAO_Endpoint *ptr = this->endpoint_; ptr != 0; ptr = ptr->next())

    if (ptr == ep)
      {
        this->endpoint_ = ep;
        return true;
      }
  return false;
}


TAO_END_VERSIONED_NAMESPACE_DECL
