// $Id$

#include "ACEXML/compass/ComponentServer.h"

using namespace Deployment;

ComponentServer::ComponentServer()
  : containers_ (0),
    activator_ (SERVER_ACTIVATOR::instance())
{}

ComponentServer::~ComponentServer()
{
  this->remove();
}

void
ComponentServer::remove()
  ACE_THROW_SPEC ((RemoveFailure))
{
  Container* entry = 0;
  for (Containers::iterator iter (*this->containers_);
       iter.next (entry) != 0;
       iter->advance())
    delete entry;
  this->activator_ = 0;
}

ComponentServer::remove_container (const Container* cref)
  ACE_THROW_SPEC ((RemoveFailure));
{
  if (this->containers_->remove (cref) != 0)
    ACE_THROW (RemoveFailure());
}
