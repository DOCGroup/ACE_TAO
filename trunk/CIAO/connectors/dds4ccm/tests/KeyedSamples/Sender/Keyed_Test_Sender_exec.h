// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "Keyed_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_Keyed_Test_Sender_Impl
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

  private:
    void start (void);
    void stop (void);

    KeyedTestConnector::Writer_var writer_;

    pulse_Generator * ticker_;
    ::Keyed_Test::CCM_Sender_Context_var context_;
    CORBA::ULong rate_;
    CORBA::UShort iterations_;
    CORBA::UShort keys_;

    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, KeyedTest_var> Keyed_Test_Table;
    Keyed_Test_Table ktests_;
    Keyed_Test_Table::iterator last_key;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Keyed_Test_Sender_Impl (void);
}

#endif /* ifndef */
