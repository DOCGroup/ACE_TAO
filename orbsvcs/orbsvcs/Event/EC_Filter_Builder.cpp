// $Id$

#include "orbsvcs/Event/EC_Filter_Builder.h"
#include "orbsvcs/Event/EC_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Filter_Builder.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Filter_Builder::~TAO_EC_Filter_Builder (void)
{
}

// ****************************************************************

TAO_EC_Null_Filter_Builder::~TAO_EC_Null_Filter_Builder (void)
{
}

TAO_EC_Filter*
TAO_EC_Null_Filter_Builder::build (
    TAO_EC_ProxyPushSupplier *,
    RtecEventChannelAdmin::ConsumerQOS&) const
{
  return new TAO_EC_Null_Filter;
}

TAO_END_VERSIONED_NAMESPACE_DECL
