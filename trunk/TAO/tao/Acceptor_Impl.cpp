// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Acceptor_Impl.cpp
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//   Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ACCEPTOR_IMPL_C
#define TAO_ACCEPTOR_IMPL_C

#include "tao/Acceptor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"

#if !defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Acceptor_Impl, "$Id$")

template <class SVC_HANDLER>
TAO_Creation_Strategy<SVC_HANDLER>::TAO_Creation_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    SVC_HANDLER (this->orb_core_),
                    -1);

  return 0;
}

template <class SVC_HANDLER>
TAO_Concurrency_Strategy<SVC_HANDLER>::TAO_Concurrency_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}


template <class SVC_HANDLER> int
TAO_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *sh,
                                                             void *arg)
{
  if (this->ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (sh) == -1)
    return -1;

  TAO_Server_Strategy_Factory *f =
    this->orb_core_->server_factory ();

  if (f->activate_server_connections ())
    return sh->activate (f->server_connection_thread_flags (),
                         f->server_connection_thread_count ());

  return this->orb_core_->reactor ()->register_handler
    (sh, ACE_Event_Handler::READ_MASK);
}

#endif /* TAO_ACCEPTOR_IMPL_C */
