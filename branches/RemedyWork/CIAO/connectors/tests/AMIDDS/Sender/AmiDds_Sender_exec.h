// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_

#include "AmiDds_SenderEC.h"
#include "tao/LocalObject.h"

#include /**/ "AmiDds_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_AmiDds_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Sender_exec_i;

  class MyFoo_callback_exec_i
    : public virtual ::AmiDds::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    // Hello Reply Handler
    virtual void hello (::CORBA::Long answer);
    virtual void hello_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);

      };

  enum UPDATER_TEST {
    UPDATE_ONE,
    SEND_SYNCH,
    UPDATE_TWO,
    SEND_ASYNCH,
    END_TEST
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
    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    void tick (void);

  private:
    void start (void);
    void stop (void);
    void add_instances_of_topic (void);
    CORBA::Boolean create_one(CORBA::Short nr);
    CORBA::Boolean update_one(CORBA::Short nr);
    CORBA::Boolean send_asynch_call(void);
    CORBA::Boolean send_synch_call(void);

    pulse_Generator * ticker_;
    ::AmiDds::CCM_Sender_Context_var context_;
    ::AmiDds::AmiDdsConnector::Updater_var updater_;
    UPDATER_TEST test_nr_;
    Atomic_Boolean test_ok_;
    TestTopicSeq topic_seq_one_;
 };
  extern "C" AMIDDS_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_AmiDds_AMI_Sender_Impl (void);
}

#endif /* ifndef */
