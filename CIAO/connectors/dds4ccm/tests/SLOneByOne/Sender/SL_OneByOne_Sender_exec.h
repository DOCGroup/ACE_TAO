// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "SL_OneByOne_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

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
    CORBA::Boolean create_one(void);
    CORBA::Boolean update_one(void);
    CORBA::Boolean delete_one(void);
    CORBA::Boolean create_many(void);
    CORBA::Boolean update_many(void);
    CORBA::Boolean delete_many(void);

    pulse_Generator * ticker_;
    ::SL_OneByOne::CCM_Sender_Context_var context_;
    CCM_DDS::TestTopic::Updater_var updater_;
    UPDATER_TEST test_nr_;
    Atomic_Boolean test_ok_;
    TestTopicSeq topic_seq_one_;
    TestTopicSeq topic_seq_many_;
 };
  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_OneByOne_Sender_Impl (void);
}

#endif /* ifndef */
