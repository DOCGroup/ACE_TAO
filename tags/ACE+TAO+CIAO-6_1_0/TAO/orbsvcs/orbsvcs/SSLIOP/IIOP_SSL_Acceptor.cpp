// $Id$

#include "orbsvcs/SSLIOP/IIOP_SSL_Acceptor.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::IIOP_SSL_Acceptor::IIOP_SSL_Acceptor (void)
  : TAO_IIOP_Acceptor (),
    base_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0)
{
}

TAO::IIOP_SSL_Acceptor::~IIOP_SSL_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

int
TAO::IIOP_SSL_Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
