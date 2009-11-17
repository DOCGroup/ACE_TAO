// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "Unkeyed_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CIAO_Unkeyed_Test_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong > Atomic_ULong;

  class Receiver_exec_i;
  /**
  * @class reader activity generator
  *
  * @brief an active object used by Receiver to perform a periodical read action
  *
  */
  class read_action_Generator
    : public ACE_Event_Handler
  {
  public:
    read_action_Generator (Receiver_exec_i &callback);

    ~read_action_Generator ();

    /// Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

  private:
    /// Maintains a handle that actually process the event
    Receiver_exec_i &pulse_callback_;

  };

  class RECEIVER_EXEC_Export UnkeyedTest_Listener_exec_i
    : public virtual ::CCM_DDS::UnkeyedTest::CCM_Listener,
      public virtual ::CORBA::LocalObject
  {
  public:
    UnkeyedTest_Listener_exec_i (Atomic_ULong &);
    virtual ~UnkeyedTest_Listener_exec_i (void);

    virtual void
    on_one_data (
      const UnkeyedTest & an_instance,
      const ::CCM_DDS::ReadInfo & info);
    virtual void
    on_many_data (
      const UnkeyedTest_Seq & an_instance,
      const ::CCM_DDS::ReadInfoSeq & info);
  private:
    Atomic_ULong &received_;
  };

  class RECEIVER_EXEC_Export PortStatusListener_exec_i
    : public virtual ::CCM_DDS::CCM_PortStatusListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    PortStatusListener_exec_i (void);
    virtual ~PortStatusListener_exec_i (void);

    virtual void
    on_requested_deadline_missed (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::RequestedDeadlineMissedStatus & status);

    virtual void
    on_sample_lost (
      ::DDS::DataReader_ptr the_reader,
      const ::DDS::SampleLostStatus & status);
  };

  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Supported operations and attributes.
    void read_one (void);
    void read_all (void);
    void get_one (void);
    void get_all (void);

    // Component attributes.
    virtual ::CORBA::ULong rate (void);

    virtual void rate (::CORBA::ULong rate);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual ::CORBA::Boolean get_data (void);

    virtual void get_data (::CORBA::Boolean get_data);

    virtual ::CORBA::Boolean read_data (void);

    virtual void read_data (::CORBA::Boolean read_data);

    virtual ::CORBA::Boolean raw_listen (void);

    virtual void raw_listen (::CORBA::Boolean raw_listen);

    // Port operations.
    virtual ::CCM_DDS::UnkeyedTest::CCM_Listener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_get_status (void);
    
    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Unkeyed_Test::CCM_Receiver_Context_var context_;
    ::CCM_DDS::UnkeyedTest::Reader_var reader_;
    ::CCM_DDS::UnkeyedTest::Getter_var getter_;

    read_action_Generator * ticker_;
    CORBA::ULong rate_;
    CORBA::UShort iterations_;
    CORBA::UShort keys_;
    CORBA::Boolean get_data_, read_data_, raw_listen_;
    long expected_;
    Atomic_ULong received_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Unkeyed_Test_Receiver_Impl (void);
}

#endif /* ifndef */

