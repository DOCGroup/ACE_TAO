// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "CSL_SRTest_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_CSL_SRTest_Sender_Impl
{
  class Sender_exec_i;

  //============================================================
  // pulse_Generator
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

  private:
    void start (void);
    void stop (void);

    ::CSL_SRTest::CCM_Sender_Context_var context_;
    pulse_Generator * ticker_;

    CORBA::ULong rate_;

    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, TestTopic_var> CSL_SRTest_Table;
    CSL_SRTest_Table _ktests_;

    void add_instance_of_topic (const char *, int x);
 };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CSL_SRTest_Sender_Impl (void);



}

#endif /* ifndef */

