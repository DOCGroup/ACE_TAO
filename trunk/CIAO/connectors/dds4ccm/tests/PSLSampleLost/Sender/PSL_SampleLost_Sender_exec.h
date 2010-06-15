// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "PSL_SampleLost_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_PSL_SampleLost_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean > Atomic_Boolean;

  class Sender_exec_i;

  //============================================================
  // Sender_exec_i
  //============================================================
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

  //============================================================
  // Sender_exec_i
  //============================================================
  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    void tick (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    // Port operations.
    virtual ::CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_test_topic_connector_status(void);

  private:
    void start (void);
    void stop (void);

    pulse_Generator * ticker_;
    ::PSL_SampleLost::CCM_Sender_Context_var context_;

    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, TestTopic_var> PSL_DLTest_Table;
    PSL_DLTest_Table _ktests_;

    void add_instance_of_topic (const char *, int x );
 };
  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_SampleLost_Sender_Impl (void);
}

#endif /* ifndef */

