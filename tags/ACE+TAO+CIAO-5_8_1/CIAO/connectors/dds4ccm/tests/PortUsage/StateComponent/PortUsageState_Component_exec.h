// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_

#include "PortUsageState_ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_PortUsageState_Test_PortUsageStateComponent_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong> Atomic_ULong;

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  class COMPONENT_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Atomic_ULong&,
                                    Atomic_ULong&);
    virtual ~ConnectorStatusListener_exec_i (void);

    virtual
    void on_inconsistent_topic (::DDS::Topic_ptr the_topic,
                                const DDS::InconsistentTopicStatus & status);
    virtual
    void on_requested_incompatible_qos (::DDS::DataReader_ptr the_reader,
                                        const DDS::RequestedIncompatibleQosStatus & status);
    virtual
    void on_sample_rejected (::DDS::DataReader_ptr the_reader,
                             const DDS::SampleRejectedStatus & status);
    virtual
    void on_offered_deadline_missed (::DDS::DataWriter_ptr the_writer,
                                     const DDS::OfferedDeadlineMissedStatus & status);
    virtual
    void on_offered_incompatible_qos (::DDS::DataWriter_ptr the_writer,
                                      const DDS::OfferedIncompatibleQosStatus & status);
    virtual
    void on_unexpected_status (::DDS::Entity_ptr the_entity,
                               ::DDS::StatusKind status_kind);
  private:
    Atomic_ULong& datareaders_count_;
    Atomic_ULong& datawriters_count_;
  };

  //============================================================
  // Component_exec_i
  //============================================================
  class COMPONENT_EXEC_Export Component_exec_i
    : public virtual PortUsageStateComponent_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Component_exec_i (void);
    virtual ~Component_exec_i (void);

    // Component attributes.
    virtual PORT_USAGE_ASSIGNMENT
    portusage_assignment (void);

    virtual void
    portusage_assignment (PORT_USAGE_ASSIGNMENT assignment);

    // Port operations.
    virtual CCM_DDS::CCM_PortStatusListener_ptr
    get_read_port_status (void);

    virtual PortUsageState_Test::PortUsageTestConnector::CCM_StateListener_ptr
    get_state_listen_port_data_listener (void);

    virtual CCM_DDS::CCM_PortStatusListener_ptr
    get_state_listen_port_status (void);

    virtual CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_connector_status ();

    virtual CCM_DDS::CCM_PortStatusListener_ptr
    get_get_port_status (void);

    virtual PortUsageState_Test::PortUsageTestConnector::CCM_Listener_ptr
    get_listen_port_data_listener (void);

    virtual CCM_DDS::CCM_PortStatusListener_ptr
    get_listen_port_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::PortUsageState_Test::CCM_PortUsageStateComponent_Context_var context_;

    PORT_USAGE_ASSIGNMENT assignment_;

    Atomic_ULong datareaders_count_;
    Atomic_ULong datawriters_count_;

    // Supported operations and attributes.
    void check_updater (void);

    void check_getter (void);

    void check_listener (void);

    void check_reader (void);

    void check_state_listen (void);
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PortUsageState_Component_Impl (void);
}

#endif /* ifndef */

