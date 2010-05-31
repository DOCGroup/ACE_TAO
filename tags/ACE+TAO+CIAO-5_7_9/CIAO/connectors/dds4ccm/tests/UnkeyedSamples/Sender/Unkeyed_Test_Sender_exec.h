// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "Unkeyed_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_Unkeyed_Test_Sender_Impl
{
  class Sender_exec_i;

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

    virtual ::CORBA::UShort rate (void);

    virtual void rate (::CORBA::UShort rate);

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

    ::Unkeyed_Test::UnkeyedTestConnector::Writer_var writer_;

    pulse_Generator * ticker_;
    ::Unkeyed_Test::CCM_Sender_Context_var context_;
    CORBA::UShort rate_;
    CORBA::UShort iterations_;
    CORBA::UShort keys_;

    TAO_SYNCH_MUTEX mutex_;
    typedef std::map<ACE_CString, UnkeyedTest_var> Unkeyed_Test_Table;
    Unkeyed_Test_Table ktests_;
    Unkeyed_Test_Table::iterator last_key;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Unkeyed_Test_Sender_Impl (void);
}

#endif /* ifndef */

