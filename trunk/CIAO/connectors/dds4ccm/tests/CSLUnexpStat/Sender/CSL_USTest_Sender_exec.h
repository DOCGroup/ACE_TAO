// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "CSL_USTest_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_CSL_USTest_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, ACE_thread_t> Atomic_ThreadId;

  class Sender_exec_i;

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  class SENDER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Atomic_Boolean &,
                                    Atomic_Boolean &,
                                    Atomic_Boolean &,
                                    Atomic_ThreadId &,
                                    Atomic_ThreadId &,
                                    Atomic_ThreadId &);

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
    void on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer, 
                                      const DDS::OfferedIncompatibleQosStatus & status);
    virtual
    void on_unexpected_status( ::DDS::Entity_ptr the_entity,
                               ::DDS::StatusKind  status_kind);

  private:
    Atomic_Boolean &subscription_matched_received_;
    Atomic_Boolean &publication_matched_received_;
    Atomic_Boolean &liveliness_changed_received_;
    Atomic_ThreadId &thread_id_subcription_matched_;
    Atomic_ThreadId &thread_id_publication_matched_;
    Atomic_ThreadId &thread_id_liveliness_changed_;
  };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_test_topic_connector_status (void);

  private:
    ::CSL_USTest::CCM_Sender_Context_var context_;

    Atomic_Boolean subscription_matched_received_;
    Atomic_Boolean publication_matched_received_;
    Atomic_Boolean liveliness_changed_received_;
    Atomic_ThreadId thread_id_listener_subscription_matched_;
    Atomic_ThreadId thread_id_listener_publication_matched_;
    Atomic_ThreadId thread_id_listener_liveliness_changed_;
 };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_USTest_Sender_Impl (void);

}

#endif /* ifndef */
