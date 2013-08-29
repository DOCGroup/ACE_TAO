// -*- C++ -*-
// $Id$

#ifndef CIAO_SL_ONEBYONE_SENDER_EXEC_FHYBU2_H_
#define CIAO_SL_ONEBYONE_SENDER_EXEC_FHYBU2_H_

#include /**/ "ace/pre.h"

#include "SL_OneByOne_SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_SL_OneByOne_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Sender_exec_i;

  enum UPDATER_TEST {
    UPDATE_CREATE,
    UPDATE_ONE,
    DELETE_ONE,
    CREATE_MANY,
    UPDATE_MANY,
    DELETE_MANY,
    END_TEST
  };

  /**
   * Write action generator
   */

  class pulse_Generator :
    public ACE_Event_Handler
  {
  public:
    pulse_Generator (Sender_exec_i &callback);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Sender_exec_i &pulse_callback_;
  };

    /**
   * ConnectorStatusListener_exec_i
   */

  class ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (Sender_exec_i &callback);
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
    Sender_exec_i &callback_;
  };

  /**
   * Component Executor Implementation Class: Sender_exec_i
   */

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_test_topic_connector_status (void);
    //@}

    //@{
    /** Operations from Components::SessionComponent. */
    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    //@{
    /** User defined public operations. */
    void tick (void);
    void start (void);
    //@}

  private:
    ::SL_OneByOne::CCM_Sender_Context_var ciao_context_;

    //@{
    /** Component attributes. */
    //@}

    //@{
    /** User defined members. */
    pulse_Generator * ticker_;

    UPDATER_TEST test_nr_;
    Atomic_Boolean test_ok_;
    TestTopicSeq topic_seq_one_;
    TestTopicSeq topic_seq_many_;

    //@}

    //@{
    /** User defined private operations. */
    void stop (void);
    void add_instances_of_topic (void);
    CORBA::Boolean create_one (void);
    CORBA::Boolean update_one (void);
    CORBA::Boolean delete_one (void);
    CORBA::Boolean create_many (void);
    CORBA::Boolean update_many (void);
    CORBA::Boolean delete_many (void);

    //@}

    /// Get the ACE_Reactor
    ACE_Reactor* reactor (void);
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_OneByOne_Sender_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
