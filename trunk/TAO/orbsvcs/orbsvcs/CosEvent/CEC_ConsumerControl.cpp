// $Id$

#include "orbsvcs/CosEvent/CEC_ConsumerControl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CEC_ConsumerControl::TAO_CEC_ConsumerControl (void)
{
}

TAO_CEC_ConsumerControl::~TAO_CEC_ConsumerControl (void)
{
}

int
TAO_CEC_ConsumerControl::activate (void)
{
  return 0;
}

int
TAO_CEC_ConsumerControl::shutdown (void)
{
  return 0;
}

void
TAO_CEC_ConsumerControl::consumer_not_exist (TAO_CEC_ProxyPushSupplier *)
{
}

void
TAO_CEC_ConsumerControl::consumer_not_exist (TAO_CEC_ProxyPullSupplier *)
{
}

void
TAO_CEC_ConsumerControl::system_exception (TAO_CEC_ProxyPushSupplier *,
                                           CORBA::SystemException &)
{
}

bool
TAO_CEC_ConsumerControl::need_to_disconnect (PortableServer::ServantBase*)
{
  return true;
}

void
TAO_CEC_ConsumerControl::successful_transmission (PortableServer::ServantBase*)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
