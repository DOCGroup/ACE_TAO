// -*- c++ -*-
//
// $Id$

void
TAO_Internal::default_svc_conf_entries (const char *resource_factory_args,
                                        const char *server_strategy_args,
                                        const char *client_strategy_args)
{
  TAO_Internal::resource_factory_args_ = resource_factory_args;
  TAO_Internal::server_strategy_args_ = server_strategy_args;
  TAO_Internal::client_strategy_args_ = client_strategy_args;
}
