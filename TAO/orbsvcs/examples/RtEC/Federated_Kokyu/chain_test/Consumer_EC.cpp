// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Counter.h"

#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "tao/ORB_Core.h"

#include "Kokyu_EC.h"
#include "Consumer.h"
#include "Service_Handler.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#include <unistd.h>
#endif //ACE_HAS_DSUI

namespace
{
  int config_run = 0;
  ACE_CString sched_type ="rms";
  FILE * ior_output_file;
}
/*
class Once_Handler: public Service_Handler
{
public:
  Once_Handler(void)
    : handled_start_(0)
    , handled_stop_(0)
  {
  }

  virtual ~Once_Handler(void)
  {
    this->reactor_->cancel_timer(this->timer_handle_);

    delete timeout_handler_impl_;
  }

  ///Takes ownership of the Timeout_Handler ONLY.
  void init (
             ACE_Reactor * reactor
             , ACE_Event_Handler *timeout_handler_impl
             , ACE_Time_Value period
             )
  {
    this->reactor_ = reactor;
    this->timeout_handler_impl_ = timeout_handler_impl;
    this->period_ = period;
  }

  virtual void handle_service_start(const RtecEventComm::EventSet&
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ))
  {
    if (this->handled_start_ == 0)
      {
        ACE_DEBUG((LM_DEBUG,"Once_Handler (%P|%t) handle_service_start() START\n"));
        this->handled_start_++; //set to true

        //WARNING: depending on Reactor, might not be a RT solution!

        this->timer_handle_ = this->reactor_->schedule_timer(this->timeout_handler_impl_,
                                                             0, //arg
                                                             ACE_Time_Value::zero, //delay
                                                             this->period_ //period
                                                             );

        ACE_DEBUG((LM_DEBUG,"Once_Handler (%P|%t) handle_service_start() END\n"));
      }
  }

  virtual void handle_service_stop(const RtecEventComm::EventSet&
                                    ACE_ENV_ARG_DECL)
  {
    if (this->handled_stop_ == 0)
      {
        ACE_DEBUG((LM_DEBUG,"Once_Handler (%P|%t) handle_service_stop() START\n"));
        this->handled_stop_++; //set to true
        ACE_DEBUG((LM_DEBUG,"Once_Handler (%P|%t) handle_service_stop() END\n"));
      }
  }

private:
  int handled_start_;
  int handled_stop_;

  ACE_Reactor *reactor_;
  ACE_Event_Handler *timeout_handler_impl_;
  ACE_Time_Value period_;
  long timer_handle_;

//   Timeout_Consumer * timeout_consumer_impl_;
//   Supplier * supplier_impl_;
//   const char * timeout_entry_point_;
//   RtecScheduler::Criticality_t crit_;
//   RtecScheduler::Importance_t imp_;
//   Kokyu_EC * kokyu_ec_;

};
*/
class Consumer_EC : public Kokyu_EC
{
public:
  Consumer_EC(void)
    : once_(0)
  {
  } //Consumer_EC()

  ~Consumer_EC(void)
  {
  } //~Consumer_EC()

  void set_up_supp_and_cons(CORBA::ORB_var
                            ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ))
  {
    ACE_Time_Value tv;

    //// CONSUMER 3 ////
    Consumer * consumer_impl3;
    ACE_NEW(consumer_impl3,
            Consumer);

    tv.set(0,50000);
    consumer_impl3->setWorkTime(tv);
    //consumer's rate specified remotely, so need to specify a period here.
    tv.set(0,200000); //period
    add_consumer(consumer_impl3,
                 "consumer3",
                 tv,
                 ACE_ES_EVENT_UNDEFINED+2,
                 RtecScheduler::VERY_HIGH_CRITICALITY,
                 RtecScheduler::VERY_HIGH_IMPORTANCE
                 ACE_ENV_ARG_PARAMETER
                 );
    ACE_CHECK;
  } //set_up_supp_and_cons()

private:
  Service_Handler * once_;
}; //class Consumer_EC

int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
   //ACE_LOG_MSG->priority_mask (LM_ERROR | LM_CRITICAL | LM_ALERT | LM_EMERGENCY, ACE_Log_Msg::PROCESS);
  //TAO_EC_Default_Factory::init_svcs ();
#ifdef ACE_HAS_DSUI
  ds_control ds_cntl("Chain_Test_Consumer","consumer_enabled.dsui");
#endif // ACE_HAS_DSUI

  TAO_EC_Kokyu_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        {
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //spawn thread to run the reactor event loop
      Reactor_Task rt;
      rt.initialize();

      // ****************************************************************

      Consumer_EC consumer_ec;
      if (consumer_ec.init(sched_type.c_str(), poa.in(), orb->orb_core()->reactor()) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Consumer_EC"), 1);
        }

      consumer_ec.set_up_supp_and_cons(orb ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************
      RtEventChannelAdmin::RtSchedEventChannel_var consumer_ec_ior =
        consumer_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(consumer_ec_ior.in()
                                                    ACE_ENV_ARG_PARAMETER);

      ACE_OS::fprintf(ior_output_file, ior.in());
      ACE_OS::fclose(ior_output_file);

      // ****************************************************************

      // At this point the consumer and supplier are connected to the
      // EC, they have provided their QoS info to the Scheduling
      // Service and the EC has informed the Scheduler about the
      // dependencies between them.
      // We can now compute the schedule for this configuration...

      // The schedule is returned in this variables....

      consumer_ec.start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

#ifdef ACE_HAS_DSUI
      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      //DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Consumer_EC (%P|%t) WORKER_ACTIVATED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 0, 0, NULL);

      //@BT
      // ACE_Object_Counter::object_id oid = ACE_OBJECT_COUNTER->increment();
      //  DSTRM_EVENT(MAIN_GROUP_FAM, START, 1, sizeof(EC_Event_Counter::event_id), (char*)&eid);
      ACE_Time_Value now(ACE_OS::gettimeofday());
      ACE_OS::printf("Consumer_EC START at %isec %iusec\n",now.sec(),now.usec());
      Object_ID oid;
      oid.pid = ACE_OS::getpid();
      oid.tid = ACE_OS::thr_self();
      DSTRM_EVENT(MAIN_GROUP_FAM, START, 0, sizeof(Object_ID), (char*)&oid);
#endif //ACE_HAS_DSUI

      rt.activate(); //need thread creation flags? or priority?
//      DSTRM_EVENT(MAIN_GROUP_FAM, START, 0, sizeof(Object_ID), (char*)&oid);

      ACE_Time_Value stop_time(310,0);
      orb->run (stop_time ACE_ENV_ARG_PARAMETER);
      //orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

#ifdef ACE_HAS_DSUI
      //@BT
      //DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Consumer_EC (%P|%t) STOP at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, sizeof(Object_ID), (char*)&oid);
#endif //ACE_HAS_DSUI

      // ****************************************************************

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Consumer_EC");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// ****************************************************************

int parse_args (int argc, char *argv[])
{
ACE_Get_Opt get_opts (argc, argv, "cs:o:");
int c;

while ((c = get_opts ()) != -1)
  switch (c)
{
 case 'o':
   ior_output_file = ACE_OS::fopen (get_opts.opt_arg (), "w");
   if (ior_output_file == 0)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "Unable to open %s for writing: %p\n",
                        get_opts.opt_arg ()), -1);
   break;
 case 's':
   sched_type = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
   break;

 case '?':
 default:
   ACE_ERROR_RETURN ((LM_ERROR,
                      "usage:  %s -s <rms|muf|edf>"
                      "\n",
                      argv [0]),
                     -1);
}
// Indicates sucessful parsing of the command line
 if (ior_output_file == 0)
   ior_output_file = ACE_OS::fopen ("/home/ron/yfzhang/iors/consumer_ec.ior", "w");
 return 0;
}
