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
  class Sender_exec_i;


class SENDER_EXEC_Export ConnectorStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_ConnectorStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    ConnectorStatusListener_exec_i (void);
    virtual ~ConnectorStatusListener_exec_i (void);
    
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
  
  };



  class pulse_Generator :
    public ACE_Event_Handler
  {
  public:
    pulse_Generator (Sender_exec_i &callback);
    /// Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    /// Maintains a handle that actually process the event
    Sender_exec_i &pulse_callback_;
  };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual ::CORBA::ULong rate (void);

    virtual void rate (::CORBA::ULong rate);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void tick ();

     // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_info_out_connector_status (void);

  private:
    void start (void);
    void stop (void);

    CCM_DDS::TestTopic::Updater_var updater_;

    pulse_Generator * ticker_;
    ::ConnectorStatusListener_Test::CCM_Sender_Context_var context_;
    CORBA::ULong rate_;
    CORBA::UShort iterations_;
    CORBA::UShort keys_;

    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, TestTopic_var> ConnectorStatusListener_Test_Table;
    ConnectorStatusListener_Test_Table ktests_;
    ConnectorStatusListener_Test_Table::iterator last_key;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ConnectorStatusListener_Test_Sender_Impl (void);



}

#endif /* ifndef */

