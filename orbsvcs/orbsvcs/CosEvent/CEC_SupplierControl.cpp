// $Id$

#include "orbsvcs/CosEvent/CEC_SupplierControl.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CEC_SupplierControl::TAO_CEC_SupplierControl (void)
{
}

TAO_CEC_SupplierControl::~TAO_CEC_SupplierControl (void)
{
}

int
TAO_CEC_SupplierControl::activate (void)
{
  return 0;
}

int
TAO_CEC_SupplierControl::shutdown (void)
{
  return 0;
}

void
TAO_CEC_SupplierControl::supplier_not_exist (TAO_CEC_ProxyPushConsumer *)
{
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_SupplierControl::supplier_not_exist (TAO_CEC_TypedProxyPushConsumer *)
{
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_SupplierControl::supplier_not_exist (TAO_CEC_ProxyPullConsumer *)
{
}

void
TAO_CEC_SupplierControl::system_exception (TAO_CEC_ProxyPullConsumer *,
                                           CORBA::SystemException &)
{
}

bool
TAO_CEC_SupplierControl::need_to_disconnect (PortableServer::ServantBase*)
{
  return true;
}

void
TAO_CEC_SupplierControl::successful_transmission (PortableServer::ServantBase*)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
