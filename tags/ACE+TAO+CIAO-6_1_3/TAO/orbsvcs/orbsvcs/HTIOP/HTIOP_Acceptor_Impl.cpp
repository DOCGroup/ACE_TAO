// $Id$

#ifndef HTIOP_ACCEPTOR_IMPL_CPP
#define HTIOP_ACCEPTOR_IMPL_CPP

#include "orbsvcs/HTIOP/HTIOP_Acceptor_Impl.h"
#include "orbsvcs/HTIOP/HTIOP_Completion_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Connector_Registry.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

#include "ace/Object_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

////////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO::HTIOP::Creation_Strategy<SVC_HANDLER>::Creation_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO::HTIOP::Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  if (sh == 0)
    {
      ACE_NEW_RETURN (sh,
                      SVC_HANDLER (this->orb_core_),
                      -1);
    }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::Accept_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open (
                          const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                          bool restart)
{
  return ACCEPT_STRATEGY_BASE::open (local_addr, restart);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
TAO::HTIOP::Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler (SVC_HANDLER *svc_handler)
{
  int const result = ACCEPT_STRATEGY_BASE::accept_svc_handler (svc_handler);
  if (result == -1)
    {
      svc_handler->remove_reference ();

      // #REFCOUNT# is zero at this point.
    }

  return result;
}

///////////////////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO::HTIOP::Concurrency_Strategy<SVC_HANDLER>::Concurrency_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO::HTIOP::Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *sh,
                                                                     void *arg)
{
  if (this->CONCURRENCY_STRATEGY_BASE::activate_svc_handler (sh,
                                                             arg) == -1)
    {
      // Activation fails, decrease reference.
      sh->remove_reference ();

      // #REFCOUNT# is zero at this point.

      return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* HTIOP_ACCEPTOR_IMPL_CPP */
