// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventService_Factory_impl.h
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTSERVICE_FACTORY_IMPL_H
#define CIAO_EVENTSERVICE_FACTORY_IMPL_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_EventService_Factory.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "tao/PortableServer/PortableServer.h"

namespace CIAO
{

  class CIAO_EVENTS_Export EventService_Factory_impl
    : public virtual EventService_Factory
  {

  public:
    // @@ George, why don't you initialize with the POA pointer. Just
    // curious.
    EventService_Factory_impl (void);

    virtual ~EventService_Factory_impl (void);

    virtual Consumer_Config_ptr create_consumer_config (EventServiceType type);

    virtual Supplier_Config_ptr create_supplier_config (EventServiceType type);

    virtual EventServiceBase * create (EventServiceType type);

    virtual void init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

    // Used to force the initialization.
    static int Initializer (void);

    //RtecEventComm::EventType find_rtec_type_id (CONNECTION_ID connection_id);

    //RtecEventComm::EventSourceID find_rtec_source_id (CONNECTION_ID connection_id);

    //EventConsumerInfo find_consumer_info (CONNECTION_ID connection_id);

    //void bind_consumer_info (EventConsumerInfo consumer_info, CONNECTION_ID connection_id);

  private:

    // @George, all these can be moved to a different
    // library. Further, they probably need to be delegated to the
    // underlying concrete type to create it for you instead of
    // providing interfaces.
    void create_rt_event_channel (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    void create_notify_channel (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    // Reference to the ORB
    CORBA::ORB_var orb_;

    // Reference to the Root POA
    PortableServer::POA_var root_poa_;

    /// Reference to the RT event channel
    RtecEventChannelAdmin::EventChannel_var rt_event_channel_;

    /// Reference to the notification channel
    CosNotifyChannelAdmin::EventChannel_var notify_channel_;

    /// Map of rt event type ids
    //ACE_Hash_Map_Manager<CONNECTION_ID, RtecEventComm::EventType, ACE_Null_Mutex>
    //  event_types_map_;

    /// Map of rt supplier ids
    //ACE_Hash_Map_Manager<CONNECTION_ID, RtecEventComm::EventSourceID, ACE_Null_Mutex>
    //  publishers_map_;

    /// Mapping of consumers to state/disconnect info.
    //ACE_Hash_Map_Manager<CIAO_Events::CONNECTION_ID,
    //                     CIAO_Events::EventConsumerInfo,
    //                     ACE_Null_Mutex> consumer_info_map_;

  };

ACE_STATIC_SVC_DECLARE (EventService_Factory_impl)
ACE_FACTORY_DECLARE (CIAO_EVENTS, EventService_Factory_impl)

}

#if defined (ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*CIAO_Module_Initializer) (void);

static CIAO_Module_Initializer
CIAO_Requires_EventService_Initializer =
  &CIAO::EventService_Factory_impl::Initializer;

#else

static int
CIAO_Requires_EventService_Initializer =
  CIAO::EventService_Factory_impl::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"
#endif /* CIAO_EVENTSERVICE_FACTORY_IMPL_H */
