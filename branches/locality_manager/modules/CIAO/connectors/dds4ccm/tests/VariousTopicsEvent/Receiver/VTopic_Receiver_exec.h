// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "VTopic_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CIAO_VTopic_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Receiver_exec_i;

  class RECEIVER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Atomic_Boolean &);
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
                               ::DDS::StatusKind  status_kind);

  private:
    Atomic_Boolean &subscription_matched_;
  };

  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

   // Supported operations and attributes.
   // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_connector_status (void);

    virtual CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual VTopic::TestTopicConn::CCM_Listener_ptr
    get_info_out_data_listener (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::VTopic::CCM_Receiver_Context_var context_;
    Atomic_Boolean subscription_matched_;
};

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTopic_Receiver_Impl (void);
}

#endif /* ifndef */
