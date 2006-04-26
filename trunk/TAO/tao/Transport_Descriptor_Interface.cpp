#include "tao/Transport_Descriptor_Interface.h"

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Descriptor_Interface.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Endpoint.h"

ACE_RCSID (tao,
           Transport_Descriptor_Interface,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Transport_Descriptor_Interface::~TAO_Transport_Descriptor_Interface (void)
{
  if (this->release_)
    {
      delete this->endpoint_;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
