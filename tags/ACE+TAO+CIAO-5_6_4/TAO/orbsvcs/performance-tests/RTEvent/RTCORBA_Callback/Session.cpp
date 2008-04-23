//
// $Id$
//
#include "Session.h"
#include "Implicit_Deactivator.h"

ACE_RCSID (TAO_RTEC_Perf_RTCORBA_Callback,
           Session,
           "$Id$")

Session::Session (Test::Callback_ptr cb,
                  PortableServer::POA_ptr poa)
  : cb_ (Test::Callback::_duplicate (cb))
  , poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
Session::sample (Test::Timestamp the_timestamp)
{
  this->cb_->sample (the_timestamp);
}

void
Session::shutdown (void)
{
  Implicit_Deactivator deactivator (this);
}

PortableServer::POA_ptr
Session::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
