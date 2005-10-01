// $Id$

#include "EC_Filter_Builder.h"
#include "EC_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Filter_Builder, "$Id$")

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
    RtecEventChannelAdmin::ConsumerQOS&
    ACE_ENV_ARG_DECL_NOT_USED) const
{
  return new TAO_EC_Null_Filter;
}
