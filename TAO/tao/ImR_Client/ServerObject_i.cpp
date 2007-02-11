// $Id$

#include "tao/ImR_Client/ServerObject_i.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ServerObject_i::ServerObject_i (CORBA::ORB_ptr orb,
                                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
ServerObject_i::ping (void)
{
}

void
ServerObject_i::shutdown (void)
{
  // Note : We want our child POAs to be able to unregister themselves from
  // the ImR, so we must destroy them before shutting down the orb.
  poa_->destroy(1, 0);
  this->orb_->shutdown (0);
}

PortableServer::POA_ptr
ServerObject_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


TAO_END_VERSIONED_NAMESPACE_DECL
