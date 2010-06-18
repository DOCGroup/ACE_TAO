// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_

#include "SL_Disabled_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include "ace/Reactor.h"

namespace CIAO_SL_Disabled_Receiver_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Receiver_exec_i;

  //============================================================
  // read_action_Generator
  //============================================================
  class read_action_Generator
    : public ACE_Event_Handler
  {
  public:
    read_action_Generator (Receiver_exec_i &callback);

    ~read_action_Generator ();

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

  private:
    Receiver_exec_i &pulse_callback_;

  };

  //============================================================
  // StateListener_exec_i
  //============================================================
  class RECEIVER_EXEC_Export StateListener_exec_i
    : public virtual ::SL_Disabled::SL_DisabledConnector::CCM_StateListener,
      public virtual ::CORBA::LocalObject
  {
  public:
    StateListener_exec_i (Atomic_Boolean &);
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
   };

  //============================================================
  // Receiver_exec_i
  //============================================================
  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Port operations.
    virtual ::SL_Disabled::SL_DisabledConnector::CCM_StateListener_ptr
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
    void read_all (void);

  private:
    ::SL_Disabled::CCM_Receiver_Context_var context_;
    read_action_Generator * ticker_;

    CORBA::ULong rate_;
    Atomic_Boolean no_operation_;
    Atomic_Boolean updater_data_;
};
  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_Disabled_Receiver_Impl (void);
}

#endif /* ifndef */
