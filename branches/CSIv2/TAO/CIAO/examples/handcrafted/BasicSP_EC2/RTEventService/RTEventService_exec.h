// $Id$

//==============================================================
/**
 * @file RTEventService_exec.h
 *
 * Header file for the actual RTEventService and RTEventServiceHome
 * component implementations.
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//==============================================================
#ifndef CIAO_RTEVENTSERVICE_EXEC_H
#define CIAO_RTEVENTSERVICE_EXEC_H

#include "RTEventServiceS.h"
#include "RTEventServiceEC.h"

#include "RTEventServiceEIC.h"
#include "tao/LocalObject.h"

#include <Event_Utilities.h>
#include <RtecEventChannelAdminC.h>
#include <RtecEventCommC.h>
#include <RtecEventCommS.h>
#include <Event/EC_Event_Channel.h>
#include <Event/EC_Default_Factory.h>

#include "tao/PortableServer/Key_Adapters.h"
#include "ace/Active_Map_Manager_T.h"

namespace MyImpl
{
  /**
   * @class RTEventService_exec_impl
   *
   * Event service executor implementation class.
   */
  class RTEVENTSERVICE_EXEC_Export RTEventService_exec_impl :
    public virtual BasicSP::RTEventService_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    RTEventService_exec_impl ();

    /// Default destructor.
    ~RTEventService_exec_impl ();

    // Operations from BasicSP::CCM_RTEventService

    virtual void connect_consumer (
        const char * event_type,
        const char * sink_name,
        const char * consumer_oid
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    virtual void connect_supplier (
        const char * event_type,
        const char * source_name,
        const char * supplier_oid
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual BasicSP::CCM_RTEventChannel_ptr
    get_rt_event_channel (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    /// Component specific context
    BasicSP::CCM_RTEventService_Context_var context_;

    // Reference to event channel
    RtecEventChannelAdmin::EventChannel_var event_channel_;

    ACE_Active_Map_Manager<
    ::RtecEventChannelAdmin::ProxyPushSupplier_var>
    proxy_supplier_map_;

    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;
  };

  /**
   * @class RTEventServiceHome_exec_impl
   *
   * RTEventService home executor implementation class.
   */
  class RTEVENTSERVICE_EXEC_Export RTEventServiceHome_exec_impl :
    public virtual BasicSP::CCM_RTEventServiceHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    RTEventServiceHome_exec_impl ();

    /// Default dtor.
    ~RTEventServiceHome_exec_impl ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

  // NEW
  class RTEventServiceSupplier_impl :
    public virtual POA_RtecEventComm::PushSupplier,
    public virtual PortableServer::RefCountServantBase
  {
  public:

    RTEventServiceSupplier_impl (void);

    RTEventServiceSupplier_impl (CORBA::ORB_ptr orb);

    virtual void disconnect_push_supplier (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    CORBA::ORB_var orb_;
  };

  class RTEventServiceConsumer_impl :
    public virtual POA_RtecEventComm::PushConsumer,
    public virtual PortableServer::RefCountServantBase
  {
  public:

    RTEventServiceConsumer_impl (void);

    RTEventServiceConsumer_impl (CORBA::ORB_ptr orb,
      Components::EventConsumerBase_ptr consumer);

    virtual void push (const RtecEventComm::EventSet& events);

    virtual void disconnect_push_consumer (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    CORBA::ORB_var orb_;
    Components::EventConsumerBase_var event_consumer_;
  };
  // END NEW

}

extern "C" RTEVENTSERVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
createRTEventServiceHome_Impl (void);

#endif /* RTEVENTSERVICE_EXEC_H */
