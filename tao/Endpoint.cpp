// $Id$

#include "tao/Endpoint.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
#include "tao/Endpoint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Endpoint::~TAO_Endpoint (void)
{
}

TAO_Endpoint *
TAO_Endpoint::next_filtered (TAO_ORB_Core *, TAO_Endpoint *root)
{
  if (root == 0)
    return this;
  return this->next();
}

TAO_END_VERSIONED_NAMESPACE_DECL

//@@ TAO_ENDPOINT_SPL_METHODS_ADD_HOOK
