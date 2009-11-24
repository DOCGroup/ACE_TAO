// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "ConnectorStatusListener_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_ConnectorStatusListener_Test_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Sender_exec_i;

class SENDER_EXEC_Export ConnectorStatusListener_sec_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_sec_exec_i (Atomic_Boolean &);
   virtual ~ConnectorStatusListener_sec_exec_i (void);

    virtual
    void on_inconsistent_topic( ::DDS::Topic_ptr the_topic,
                                const DDS::InconsistentTopicStatus & status);
    virtual
    void on_requested_incompatible_qos( ::DDS::DataReader_ptr the_reader,
                                        const DDS::RequestedIncompatibleQosStatus & status);
    virtual
    void on_sample_rejected( ::DDS::DataReader_ptr the_reader,
                             const DDS::SampleRejectedStatus & status);
    virtual
      void on_offered_deadline_missed( ::DDS::DataWriter_ptr the_writer,
                                     const DDS::OfferedDeadlineMissedStatus & status);
    virtual
    void on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer,
                                      const DDS::OfferedIncompatibleQosStatus & status);
    virtual
      void on_unexpected_status( ::DDS::Entity_ptr the_entity,
       ::DDS::StatusKind  status_kind);
     private:
    Atomic_Boolean &inconsistent_;

  };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void add_instance_of_sec_topic (const char *, int x );


    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

     // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
      get_test_sec_topic_connector_status(void);

  private:
    ::ConnectorStatusListener_Test::CCM_Sender_Context_var context_;

    Atomic_Boolean inconsistent_;

    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, TestSecondTopic_var> ConnectorStatusListener_TestSec_Table;
    ConnectorStatusListener_TestSec_Table sec_ktests_;
 };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ConnectorStatusListener_Test_Sender_Impl (void);



}

#endif /* ifndef */

