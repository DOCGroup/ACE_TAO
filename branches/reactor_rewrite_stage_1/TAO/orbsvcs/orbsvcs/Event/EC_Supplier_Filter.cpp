// $Id$

#include "EC_Supplier_Filter.h"
#include "EC_ProxySupplier.h"
#include "EC_QOS_Info.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Supplier_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Supplier_Filter, "$Id$")

TAO_EC_Supplier_Filter::~TAO_EC_Supplier_Filter (void)
{
}

// ****************************************************************

void
TAO_EC_Filter_Worker::work (TAO_EC_ProxyPushSupplier *supplier
                            TAO_ENV_ARG_DECL)
{
  TAO_EC_QOS_Info qos_info = this->event_info_;
  supplier->filter (this->event_, qos_info TAO_ENV_ARG_PARAMETER);
}
