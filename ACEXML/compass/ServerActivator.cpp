// $Id$


#if defined (__ACE_INLINE__)
#include "ServerActivator.inl"
#endif /* __ACE_INLINE__ */

#include "ACEXML/compass/ServerActivator.h"

using namespace Deployment;


ServerActivator::ServerActivator()
{
  ACE_NEW (this->comp_servers_, ComponentServers());
}

ServerActivator::~ServerActivator()
{
  ComponentServer** entry = 0;
  for (ComponentServers::iterator iter (*this->comp_servers_);
       iter.next (entry) != 0;
       iter->advance())
    delete *entry;
}

ComponentServer*
ServerActivator::create_component_server (ConfigValues* config)
  ACE_THROW_SPEC ((CreateFailure, InvalidConfiguration))
{
  ComponentServer* cserver = 0;
  ACE_NEW_RETURN (cserver, ComponentServer (config), 0);
  if (this->comp_servers_->insert (cserver) < 0)
    ACE_THROW (CreateFailure());
  return cserver;
}

void
ServerActivator::remove_component_server (ComponentServer* server)
  ACE_THROW_SPEC ((RemoveFailure))
{
  if (this->comp_servers_->remove (server) != 0)
    ACE_THROW (RemoveFailure());
  delete server;
  return;
}
