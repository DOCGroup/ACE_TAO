// $Id$


#include "Sender_exec.h"
#include "ace/OS_NS_time.h"


namespace CIAO_Sender_Impl
{
  //============================================================
  // Callback class TimeTrigger
  //============================================================
  TTC_Callback_exec_i::TTC_Callback_exec_i (Sender_exec_i& sender_comp_callback)
  : sender_comp_callback_ (sender_comp_callback)
  {
  }

  TTC_Callback_exec_i::~TTC_Callback_exec_i (void)
  {
  }

  void
  TTC_Callback_exec_i::on_timer ( const TTC::timeout_t &t)

  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("TTC_Callback::on_timer,")
                ACE_TEXT("received callback from TimeTrigger Connector.")
                ACE_TEXT("t.seconds = %u, t.nanosec = %u\n"),
                t.time_val.seconds, t.time_val.nanosec));
    this->sender_comp_callback_.do_something();
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i ()
  : nr_trigger_(0)
    , timer_id_ (0)
    , service_id_ ("TimeService")
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.
  void
  Sender_exec_i::start(void)
  {
    ACE_OS::sleep(2);

    // set delaytime and rate of  a periodic TimeTrigger
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::start start_scheduler_periodic.\n"));
    TTC_Callback_var  cb = new TTC_Callback_exec_i (*this);

    TTC::timeout_t delay;
    delay.flag = TTC::RELATIVE_TIME;
    delay.time_val.seconds= 2;
    delay.time_val.nanosec= 0;

    TTC::timeout_t rate;
    rate.flag = TTC::RELATIVE_TIME;
    rate.time_val.seconds= 0;
    rate.time_val.nanosec= 500000000;


    try {
      // get reference of TTC_Scheduler Facet of TimeTrigger connector.
      CORBA::Object_var test_ref = this->context_->resolve_service_reference(this->service_id_);

      if (CORBA::is_nil(test_ref.in()))
        {
          ACE_ERROR ((LM_ERROR,
                        "Sender_exec_i::start - "
                        "objref is NIL\n"));
        }
      else
        {
          // narow object ref to TTC_Scheduler
        TTC_Scheduler_var timetrigger =  TTC_Scheduler::_narrow (test_ref.in ());
          if (CORBA::is_nil(timetrigger .in()))
            {
              ACE_ERROR ((LM_ERROR,
                         "Sender_exec_i::start - "
                         "timetrigger_ is NIL\n"));
            }
          else
            this->timer_id_ = timetrigger->start_scheduler_periodic(delay,rate, cb.in());
         }
      }

    catch (const ::Components::CCMException &ex)
      {
        ACE_ERROR ((LM_ERROR,
                        "Resolve_service_reference - "
                        "Caught exception\n"));
      }
  }

  void
  Sender_exec_i::do_something(void)
  {
    this->nr_trigger_++;
    ACE_DEBUG ((LM_DEBUG, "Sender received time trigger nr %u!.\n",
                          this->nr_trigger_));
    TTC_Scheduler_var timetrigger;

    //after a while cancel periodic Timetrigger and use sporadic Timetrigger
    if ((this->nr_trigger_ > 5) && (this->nr_trigger_ < 7))
    {
      try {
        // get reference of TTC_Scheduler Facet of TimeTrigger connector.
        CORBA::Object_var test_ref = this->context_->resolve_service_reference(this->service_id_);

        if (CORBA::is_nil(test_ref.in()))
          {
            ACE_ERROR ((LM_ERROR,
                          "Sender_exec_i::do_something - "
                          "objref is NIL\n"));
          }
        else
          {
            // narow object ref to TTC_Scheduler
            timetrigger =  TTC_Scheduler::_narrow (test_ref.in ());
            if (CORBA::is_nil(timetrigger .in()))
              {
                ACE_ERROR ((LM_ERROR,
                           "Sender_exec_i::do_something - "
                           "timetrigger_ is NIL\n"));
              }
            else
              timetrigger->cancel_scheduler(this->timer_id_);
          }
        }
      catch (const ::Components::CCMException &ex)
        {
          ACE_ERROR ((LM_ERROR,
                          "Resolve_service_reference - "
                          "Caught exception\n"));
        }

      //use sporadic Timertrigger, which fired 3 seconds after now.
      TTC_Callback_var  cb = new TTC_Callback_exec_i (*this);

      TTC::timeout_t time;
      time.flag = TTC::RELATIVE_TIME;
      time.time_val.seconds = 3;
      this->timer_id_ = timetrigger->start_scheduler_sporadic(time, cb.in());
    }
  }

  // Component attributes and port operations.

  // Operations from Components::SessionComponent.
  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_activate \n"));
    start ();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Sender_Impl (void)
  {

   ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i ());

    return retval;
  }
}

