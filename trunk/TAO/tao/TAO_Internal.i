// -*- c++ -*-
//
// $Id$

void
TAO_Internal::_svc_conf (const char *resource_factory_args,
                         const char *server_strategy_args,
                         const char *client_strategy_args)
{
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
  TAO_Internal::resource_factory_args_ = resource_factory_args;
  TAO_Internal::server_strategy_args_ = server_strategy_args;
  TAO_Internal::client_strategy_args_ = client_strategy_args;
#else /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
  ACE_UNUSED_ARG (resource_factory_args);
  ACE_UNUSED_ARG (server_strategy_args);
  ACE_UNUSED_ARG (client_strategy_args);
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
}
