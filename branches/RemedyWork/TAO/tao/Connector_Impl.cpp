// $Id$

#ifndef TAO_CONNECTOR_IMPL_CPP
#define TAO_CONNECTOR_IMPL_CPP

#include "tao/Connector_Impl.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class SVC_HANDLER>
TAO_Connect_Creation_Strategy<SVC_HANDLER>::
    TAO_Connect_Creation_Strategy (ACE_Thread_Manager* t,
                                   TAO_ORB_Core *orb_core)
    : ACE_Creation_Strategy <SVC_HANDLER> (t, orb_core->reactor ()),
      orb_core_ (orb_core)
{

}

template <class SVC_HANDLER> int
TAO_Connect_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    SVC_HANDLER (this->orb_core_),
                    -1);

  sh->transport ()->opened_as (TAO::TAO_CLIENT_ROLE);

  // At this point, the #REFCOUNT# is one.

  return 0;
}


////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO_Connect_Concurrency_Strategy<SVC_HANDLER>::
    TAO_Connect_Concurrency_Strategy (TAO_ORB_Core *orb_core)
    : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO_Connect_Concurrency_Strategy<SVC_HANDLER>::
    activate_svc_handler (SVC_HANDLER *sh, void *arg)
{
  return ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (sh, arg);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_CONNECTOR_IMPL_CPP */
