// $Id$

#include "tao/Remote_Object_Proxy_Broker.h"

ACE_RCSID (tao, TAO_Object_Remote_Proxy_Broker, "$Id$")

TAO_Remote_Object_Proxy_Broker *
the_tao_remote_object_proxy_broker (void)
{
  static TAO_Remote_Object_Proxy_Broker the_broker;
  return &the_broker;
}

TAO_Remote_Object_Proxy_Broker::TAO_Remote_Object_Proxy_Broker (void)
{
  // No-Op.
}

TAO_Remote_Object_Proxy_Broker::~TAO_Remote_Object_Proxy_Broker (void)
{
  // No-Op.
}

TAO_Object_Proxy_Impl &
TAO_Remote_Object_Proxy_Broker::select_proxy (CORBA::Object_ptr
                                              ACE_ENV_ARG_DECL_NOT_USED)
{
  return this->remote_proxy_impl_;
}

