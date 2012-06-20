// -*- C++ -*-
// $Id$

#ifndef CIAO_TTCONNECTOR_EXEC_H_
#define CIAO_TTCONNECTOR_EXEC_H_


#include "TTSConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "TTSConnector_conn_export.h"
#include "tao/LocalObject.h"


namespace CIAO_TTSConnector_Impl
{
   class TTSConnector_conn_i;

  //============================================================
   // pulse_Generator
   //============================================================
   class pulse_Generator : public ACE_Event_Handler
   {
   public:
     pulse_Generator (TTC_Callback_ptr callback);

     virtual ~pulse_Generator ();

     virtual int handle_timeout (const ACE_Time_Value &tv,
                                 const void *arg);
  private:
     TTC_Callback_var pulse_callback_;
  };

   //============================================================
   // Facet Executor Implementation Class of TimeTriggerConnector:
   //  TTC_Scheduler_exec_i
   //============================================================
   class TTSCONNECTOR_EXEC_Export TTC_Scheduler_exec_i
     : public virtual ::CCM_TTC_Scheduler,
       public virtual ::CORBA::LocalObject
   {
   public:
     TTC_Scheduler_exec_i (TTSConnector_conn_i& component);

     virtual ~TTC_Scheduler_exec_i (void);

     virtual TTC::TimerId  start_scheduler_periodic (const TTC::timeout_t &delay_time,
                                                     const TTC::timeout_t &rate,
                                                     TTC_Callback_ptr cb);
     virtual TTC::TimerId  start_scheduler_sporadic (const TTC::timeout_t &time,
                                                     TTC_Callback_ptr cb);
     virtual void cancel_scheduler (TTC::TimerId timer_id);

   private:
     TTSConnector_conn_i& component_;
   };


  class TTSCONNECTOR_EXEC_Export TTSConnector_conn_i
    : public virtual TTSConnector_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    TTSConnector_conn_i (void);
    virtual ~TTSConnector_conn_i (void);

    //@{
    /** User defined operations */
    virtual
    long start_reactor (CORBA::Long delay,
                        CORBA::ULong rate,
                        TTC_Callback_ptr cb);

    virtual
    void
    cancel_reactor (CORBA::Long timer_id);

    //@}

    //@{
    /** Component attributes and port operations. */
    virtual ::CCM_TTC_Scheduler_ptr
    get_p_provides_tcc_scheduler (void);
    //@}

    //Operations from Components::SessionComponent
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    /** @name User defined members. */
    //@{
    pulse_Generator * ticker_;
    //@}

    /// Context for component instance. Used for all middleware communication
    ::CCM_TTSConnector_Context_var context_;

    /** @name Component facets. */
    //@{
    //@}

    /** @name User defined private operations. */
     //@{
    /// Get the ACE_Reactor
    ACE_Reactor* reactor (void);
    //@}

  };
  extern "C" TTSCONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_TTSConnector_Impl (void);
}

#endif /* ifndef */
