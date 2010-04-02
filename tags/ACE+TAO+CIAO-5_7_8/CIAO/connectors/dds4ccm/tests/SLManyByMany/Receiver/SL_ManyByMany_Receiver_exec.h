// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "SL_ManyByMany_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CIAO_SL_ManyByMany_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Receiver_exec_i;
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
  class RECEIVER_EXEC_Export StateListener_exec_i
    : public virtual ::CCM_DDS::TestTopic::CCM_StateListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    StateListener_exec_i (Atomic_Boolean &, Atomic_Boolean &, Atomic_Boolean &,
                          Atomic_Boolean &, Atomic_Boolean &, Atomic_Boolean &);
    virtual ~StateListener_exec_i (void);

    virtual void
      on_creation (
        const ::TestTopic & datum,
        const ::CCM_DDS::ReadInfo & info);
    virtual void
      on_one_update (
        const ::TestTopic & datum,
        const ::CCM_DDS::ReadInfo & info);
     virtual void
      on_many_updates (
        const ::TestTopicSeq & data,
        const ::CCM_DDS::ReadInfoSeq & infos);
    virtual void
      on_deletion (
        const ::TestTopic & datum,
        const ::CCM_DDS::ReadInfo & info);
  private:
     Atomic_Boolean &no_operation_;
     Atomic_Boolean &on_creation_;
     Atomic_Boolean &on_many_update_;
     Atomic_Boolean &on_deletion_;
     Atomic_Boolean &create_data_;
     Atomic_Boolean &update_data_;

 };

  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Port operations.
    virtual ::CCM_DDS::TestTopic::CCM_StateListener_ptr
    get_info_out_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_out_status (void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    void read_all(void);

  private:
    ::SL_ManyByMany::CCM_Receiver_Context_var context_;
    ::CCM_DDS::TestTopic::Reader_var reader_;
    read_action_Generator * ticker_;
    CORBA::ULong rate_;
    Atomic_Boolean no_operation_;
    Atomic_Boolean on_creation_;
    Atomic_Boolean on_many_update_;
    Atomic_Boolean on_deletion_;
    Atomic_Boolean create_data_;
    Atomic_Boolean update_data_;
    Atomic_Boolean reader_data_;

};
  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_ManyByMany_Receiver_Impl (void);
}

#endif /* ifndef */
