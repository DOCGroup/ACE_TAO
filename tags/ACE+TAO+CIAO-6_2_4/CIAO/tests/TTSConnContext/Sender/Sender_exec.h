// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"
#include "../TTSCConnector/TTSCConnectorC.h"


namespace CIAO_Sender_Impl
{
  class Sender_exec_i;

  class TTC_Callback_exec_i
     : public TTC_Callback,
       public virtual ::CORBA::LocalObject
     {
     public:
    TTC_Callback_exec_i(Sender_exec_i& sender_comp_callback);
       virtual ~TTC_Callback_exec_i (void);

       virtual void
       on_timer (const TTC::timeout_t &time);

     private:
        Sender_exec_i& sender_comp_callback_;

     };

 class SENDER_EXEC_Export Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i ();
    virtual ~Sender_exec_i (void);

    //@{

    /** Supported operations and attributes. */
    virtual void
    start(void);

    virtual void
    do_something(void);
    //@}

    //@{
    /** Component attributes and port operations. */

    //@}

    //@{
    /** Operations from Components::SessionComponent. */
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

  private:
    CCM_Sender_Context_var context_;
    CORBA::UShort nr_trigger_;
    //Id of used Timetrigger
    TTC::TimerId timer_id_;
    ::CORBA::String_var TimerService_;
    const char *service_id_;

  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Sender_Impl (void);
}

#endif /* ifndef */

