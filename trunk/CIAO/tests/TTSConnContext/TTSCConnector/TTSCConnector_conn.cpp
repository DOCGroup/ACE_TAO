// $Id$

#include "tao/ORB_Core.h"
#include "TTSCConnector_conn.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"
#include "ace/Reactor.h"

namespace CIAO_TTSCConnector_Impl
{

  //Implementation of a timer, could be replaced with something else
  //============================================================
  // pulse_Generator
  //============================================================
  pulse_Generator::pulse_Generator (TTC_Callback_ptr callback)
    :pulse_callback_ (TTC_Callback::_duplicate (callback))
  {
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &tv, const void *)
  {
    TTC::TimeT ttime;
    ttime.seconds = tv.sec();
    ttime.nanosec = tv.usec() * 1000;
    TTC::timeout_t tt;
    tt.flag = TTC::ABSOLUTE;
    tt.time_val = ttime;

    ACE_DEBUG ((LM_DEBUG,
                "pulse_Generator::handle_timeout - "
                "connector activate callback TimeTrigger.\n"));

    this->pulse_callback_->on_timer(tt);
    return 0;
  }

  ///============================================================
  // Facet Executor Implementation Class: TTC_Scheduler_exec_i
  //============================================================
  TTC_Scheduler_exec_i::TTC_Scheduler_exec_i (TTSCConnector_conn_i& component )
  : component_ (component)
  {
  }

  TTC_Scheduler_exec_i::~TTC_Scheduler_exec_i (void)
  {
  }

  // Operations and attributes from ::TTC_Scheduler
  TTC::TimerId
  TTC_Scheduler_exec_i::start_scheduler_periodic (
                                           const TTC::timeout_t & delay_time,
                                           const TTC::timeout_t & rate,
                                           TTC_Callback_ptr cb)
  {
    ACE_DEBUG ((LM_DEBUG,
                "TTC_Scheduler_exec_i::start_scheduler_periodic called, "
                "delaytime = %u sec, rate = %u nanosec\n",
                delay_time.time_val.seconds, rate.time_val.nanosec));

    TTC_Callback_var cb_ = TTC_Callback::_duplicate(cb);

    // start a timer implementaton, in this example the ACE_reactor timer is used.
    return this->component_.start_reactor(delay_time.time_val.seconds,
                                          rate.time_val.nanosec, cb_.in());
  }

  TTC::TimerId
  TTC_Scheduler_exec_i::start_scheduler_sporadic (const TTC::timeout_t & time,
                                                  TTC_Callback_ptr cb)

  {
    ACE_DEBUG ((LM_DEBUG,
                "TTC_Scheduler_exec_i::start_scheduler_sporadic called,"
                "time.seconds = %u\n",
                time.time_val.seconds));
    // start a timer implementaton, in this example the ACE_reactor timer
    // is used, which only can handle time-intervals and no absolute timestamp.
    if (time.flag == TTC::ABSOLUTE)
      ACE_ERROR ((LM_ERROR,
                  "TTC_Scheduler_exec_i::start_scheduler_sporadic "
                  "can't handle absolute time.\n"));
    TTC_Callback_var cb_ = TTC_Callback::_duplicate(cb);
    return this->component_.start_reactor(time.time_val.seconds, 0, cb_.in());
  }

  void TTC_Scheduler_exec_i::cancel_scheduler (TTC::TimerId timer_id)
  {
    ACE_DEBUG ((LM_DEBUG,
                  "TTC_Scheduler_exec_i::cancel_scheduler called\n"));
    this->component_.cancel_reactor(timer_id);
  }

  //============================================================
  // Component Executor Implementation Class: TTSCConnector_conn_i
  //============================================================
  TTSCConnector_conn_i::TTSCConnector_conn_i ()
  {
  }

  TTSCConnector_conn_i::~TTSCConnector_conn_i (void)
  {
  }

  // Supported operations and attributes.
  ACE_Reactor*
  TTSCConnector_conn_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object =
      this->context_->get_CCM_object();
    if (! ::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (! ::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }
    if (reactor == 0)
      {
        throw ::CORBA::INTERNAL ();
      }
    return reactor;
  }

  long
  TTSCConnector_conn_i::start_reactor (CORBA::Long delay, CORBA::ULong rate,
                                      TTC_Callback_ptr cb)
   {
    this->ticker_ = new pulse_Generator (cb);

    // calculate the interval time
    long usec = 0;
    if (rate != 0)
      usec = rate/1000;
    long timer_id =
         this->reactor ()->schedule_timer (
                              this->ticker_,
                              0,
                              ACE_Time_Value (delay, 0), //delay
                              ACE_Time_Value (0, usec));  //rate
     if ( timer_id == -1)
       {
         ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("TTSCConnector_conn_i::start_reactor\n")));
       }
     return timer_id;
   }

  void
  TTSCConnector_conn_i::cancel_reactor (CORBA::Long timer_id)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TTSCConnector_conn_i::cancel_reactor\n")));
    this->reactor ()->cancel_timer (timer_id);
  }

  // Component attributes and port operations.
  CCM_TTC_Scheduler_ptr
  TTSCConnector_conn_i::get_p_provides_tcc_scheduler (void)
    {
      return ( new TTC_Scheduler_exec_i (*this) );
    }

  // Operations from Components::SessionComponent.
  void
  TTSCConnector_conn_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
     this->context_ =
       CCM_TTSCConnector_Context::_narrow (ctx);


    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  TTSCConnector_conn_i::configuration_complete (void)
  {
  }

  void
  TTSCConnector_conn_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "TTSConnector_conn_i::ccm_activate \n"));

    const char * service_id = ACE_OS::strdup("TimeService");
    try {

      TTC_Scheduler_var facet_connector_ = this->get_p_provides_tcc_scheduler();
      if (CORBA::is_nil(facet_connector_.in()))
        {
          ACE_ERROR ((LM_ERROR,
                          "TTSCConnector_conn_i::ccm_activate - "
                          "facet_connector_ is NIL\n"));
        }
      // install reference of port.
      this->context_->install_service_component_reference (
             service_id,
             facet_connector_.in());
    }
    catch (const ::Components::CCMException &ex)
    {
      ACE_ERROR ((LM_ERROR,
                       "TTSCConnector_conn_i::ccm_activate -"
                       "install_service_component_reference "
                       "caught exception\n"));
    }
  }

  void
  TTSCConnector_conn_i::ccm_passivate (void)
  {
    this->reactor ()->cancel_timer (this->ticker_);
  }

  void
  TTSCConnector_conn_i::ccm_remove (void)
  {
  }
extern "C" TTSCONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_TTSCConnector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      TTSCConnector_conn_i);

    return retval;
  }
}
