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

#include "CIAO_ValueC.h"

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

    // Operations from RtecEventChannel::CCM_EventChannel

    virtual ::RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    destroy (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (
    RtecEventChannelAdmin::Observer_ptr gw
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER
      ));

    virtual void
    remove_observer (
    RtecEventChannelAdmin::Observer_Handle gw
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER
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

    virtual RtecEventChannelAdmin::CCM_EventChannel_ptr
    get_rt_event_channel (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    /// Component specific context
    BasicSP::CCM_RTEventService_Context_var context_;

    // Reference to event channel
    RtecEventChannelAdmin::EventChannel_var event_channel_;

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

  class RTEVENTSERVICE_EXEC_Export RTEventServiceSupplier_impl :
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

  class RTEVENTSERVICE_EXEC_Export RTEventServiceConsumer_impl :
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

}

namespace CIAO
{

  class RTEVENTSERVICE_EXEC_Export Object_Reference_Cookie
    : public virtual OBV_CIAO::Cookie
  {

  public:
    Object_Reference_Cookie ();

    Object_Reference_Cookie (CORBA::Object_ptr obj);

    virtual ::CORBA::OctetSeq * get_cookie (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    ~Object_Reference_Cookie ();

    int insert (CORBA::Object_ptr obj);

    static int extract (::Components::Cookie *c,
                        CORBA::Object_ptr obj);
  };

  class Object_Reference_Cookie_init : public virtual ::Components::Cookie_init
  {

  public:
    Object_Reference_Cookie_init (void);

    virtual ~Object_Reference_Cookie_init (void);

    virtual CORBA::ValueBase * create_for_unmarshal (void);
  };

}

extern "C" RTEVENTSERVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
createRTEventServiceHome_Impl (void);

#endif /* RTEVENTSERVICE_EXEC_H */
