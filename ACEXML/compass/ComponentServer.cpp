// $Id$

#if defined (__ACE_INLINE__)
#include "ComponentServer.inl"
#endif /* __ACE_INLINE__ */

#include "ACEXML/compass/ComponentServer.h"

using namespace Deployment;

ComponentServer::ComponentServer(ConfigValues* config)
  : containers_ (0),
    config_ (config),
    activator_ (SERVER_ACTIVATOR::instance())
{}

ComponentServer::~ComponentServer()
{
  this->remove();
}

Container*
ComponentServer::create_container (ConfigValues* config)
  ACE_THROW_SPEC ((CreateFailure, InvalidConfiguration))
{
  Container* ctr = 0;
  ACE_NEW_RETURN (ctr, Container (config), 0);
  if (this->containers_->insert (ctr) != 0)
    ACE_THROW (CreateFailure());
  return ctr;
}

void
ComponentServer::remove_container (Container* cref)
  ACE_THROW_SPEC ((RemoveFailure));
{
  if (this->containers_->remove (cref) != 0)
    ACE_THROW (RemoveFailure());
  delete cref;
}

void
ComponentServer::remove()
  ACE_THROW_SPEC ((RemoveFailure))
{
  Container** entry = 0;
  for (Containers::iterator iter (*this->containers_);
       iter.next (entry) != 0;
       iter->advance())
    delete *entry;

  ConfigValues** value = 0;
  for (ConfigValues::iterator iter (*this->config_);
       iter.next (value) != 0;
       iter->advance())
    delete *value;

  this->activator_ = 0;
}
