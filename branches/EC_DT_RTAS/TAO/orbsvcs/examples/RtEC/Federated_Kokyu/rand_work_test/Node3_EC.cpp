// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Select_Reactor_Base.h" //for ACE_Select_Reactor_Impl::DEFAULT_SIZE
#include "ace/Map.h"
#include "ace/Vector_T.h"
#include "ace/Reactor.h"

#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Limit.h"

#include "Kokyu_EC.h"
#include "Consumer.h"
#include "Supplier.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#endif //ACE_HAS_DSUI

#include <sstream>

namespace
{
  int config_run = 0;
  ACE_CString sched_type ="rms";
  FILE * ior_output_file;

  typedef ACE_Vector<const char*> Filename_Array;
  Filename_Array ior_input_files;
}

class Supplier_EC : public Kokyu_EC
{
  //need to handle multiple gateways!
  typedef ACE_Map_Manager<const char*,TAO_EC_Gateway_Sched*,ACE_Thread_Mutex> Gateway_Map;

  Gateway_Map gateways;
public:
  Supplier_EC()
  {
    this->gateways.open();
  } //Supplier_EC()

  ~Supplier_EC(void)
  {
    //TODO: close down gateways

    //now delete them and their IOR strings
    Gateway_Map::iterator iter = this->gateways.begin();
    Gateway_Map::iterator done = this->gateways.end();
    while(iter != done)
      {
        Gateway_Map::ENTRY entry = *iter;

        Gateway_Map::KEY key = entry.ext_id_;
        Gateway_Map::VALUE val = entry.int_id_;
        //we can delete as long as we don't unbind
        //don't delete keys since we didn't allocate them
        delete key;
        delete val;

        ++iter;
      }
    this->gateways.unbind_all();

    this->gateways.close();
  } //~Supplier_EC()

  void init_gateway(CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    const char* consumer_ec_ior)
  {
    if (this->gateways.find(consumer_ec_ior) == 0)
      {
        //Already a gateway for that EC
        ACE_DEBUG((LM_DEBUG,"Supplier_EC (%P|%t) init_gateway(): Tried to create already-existing gateway for %s\n",
                   consumer_ec_ior));
        return;
      }

      CORBA::Object_var obj;
      RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec, consumer_ec;
      ACE_CHECK;

      obj = orb->string_to_object(consumer_ec_ior ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      //WARNING: if consumer_ec_ior is not a readable file, obj is null!
      consumer_ec = RtEventChannelAdmin::RtSchedEventChannel::_narrow(obj.in()
                                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::EventChannel_var supplier_event_channel =
        this->event_channel(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::EventChannel_var consumer_event_channel =
        consumer_ec->event_channel(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::Scheduler_var supplier_scheduler =
        this->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::Scheduler_var consumer_scheduler =
        consumer_ec->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      TAO_EC_Gateway_Sched *gateway;
      ACE_NEW(gateway,
              TAO_EC_Gateway_Sched() );

      gateway->init(supplier_event_channel.in(),
                    consumer_event_channel.in(),
                    supplier_scheduler.in(),
                    consumer_scheduler.in(),
                    "gateway1", consumer_ec_ior
                    ACE_ENV_ARG_PARAMETER);

      this->gateways.bind(consumer_ec_ior,gateway);

      ACE_CHECK;
            PortableServer::ObjectId_var gateway_oid =
         poa->activate_object(gateway ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Object_var gateway_obj =
         poa->id_to_reference(gateway_oid.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

       RtecEventChannelAdmin::Observer_var obs =
         RtecEventChannelAdmin::Observer::_narrow(gateway_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::Observer_Handle local_ec_obs_handle =
         consumer_event_channel->append_observer (obs.in () ACE_ENV_ARG_PARAMETER);
      ACE_UNUSED_ARG(local_ec_obs_handle);
      ACE_CHECK;

      consumer_ec->start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
  } //init_gateway()

  void set_up_supp_and_cons (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ))
  {
    // P 3

    subtask_t subt;
    // T_1_3 122 849 335
    subt.task_index = 1;
    subt.subtask_index = 3;
    subt.exec = ACE_Time_Value(0,122);
    subt.period = ACE_Time_Value(0,849);
    subt.phase = ACE_Time_Value(0,335);

    int supp_id = 100*subt.task_index + subt.subtask_index;
    int next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_middle_subtask(subt,supp_id,
                         supp_id,next_supp_id //use same val for event type and supplier ids
                         ACE_ENV_ARG_PARAMETER);

    // T_4_2 30 257 132
    subt.task_index = 4;
    subt.subtask_index = 2;
    subt.exec = ACE_Time_Value(0,30);
    subt.period = ACE_Time_Value(0,257);
    subt.phase = ACE_Time_Value(0,132);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_middle_subtask(subt,supp_id,
                         supp_id,next_supp_id //use same val for event type and supplier ids
                         ACE_ENV_ARG_PARAMETER);

    // T_5_1 34 952 501
    subt.task_index = 5;
    subt.subtask_index = 1;
    subt.exec = ACE_Time_Value(0,34);
    subt.period = ACE_Time_Value(0,952);
    subt.phase = ACE_Time_Value(0,501);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_first_subtask(subt,supp_id,next_supp_id,
                         supp_id,next_supp_id //use same val for event type and supplier ids
                         ACE_ENV_ARG_PARAMETER);

    // T_7_2 5 110 26
    subt.task_index = 7;
    subt.subtask_index = 2;
    subt.exec = ACE_Time_Value(0,5);
    subt.period = ACE_Time_Value(0,110);
    subt.phase = ACE_Time_Value(0,26);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_middle_subtask(subt,supp_id,
                         supp_id,next_supp_id //use same val for event type and supplier ids
                         ACE_ENV_ARG_PARAMETER);

    // T_10_3 107 703 673
    subt.task_index = 10;
    subt.subtask_index = 3;
    subt.exec = ACE_Time_Value(0,107);
    subt.period = ACE_Time_Value(0,703);
    subt.phase = ACE_Time_Value(0,673);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_middle_subtask(subt,supp_id,
                         supp_id,next_supp_id //use same val for event type and supplier ids
                         ACE_ENV_ARG_PARAMETER);

    // T_12_1 15 190 168
    subt.task_index = 12;
    subt.subtask_index = 1;
    subt.exec = ACE_Time_Value(0,15);
    subt.period = ACE_Time_Value(0,190);
    subt.phase = ACE_Time_Value(0,168);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_first_subtask(subt,supp_id,next_supp_id,
                         supp_id,next_supp_id //use same val for event type and supplier ids
                         ACE_ENV_ARG_PARAMETER);

    //Kokyu_EC::start(ACE_ENV_SINGLE_ARG_PARAMETER);
    //ACE_CHECK;
  } //set_up_supp_and_cons()
};

int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
  //TAO_EC_Default_Factory::init_svcs ();
#ifdef ACE_HAS_DSUI
  ds_control* ds_cntl = new ds_control ("Fan_Test_Supplier","supplier_enabled.dsui");
#endif //ACE_HAS_DSUI

  TAO_EC_Kokyu_Factory::init_svcs ();
  TAO_EC_Gateway_IIOP_Factory::init_svcs ();

  //@BT
  //DSTRM_EVENT(MAIN_GROUP_FAM, START,1,0,NULL);
  ACE_Time_Value now(ACE_OS::gettimeofday());
  ACE_OS::printf("Supplier_EC START at %isec %iusec\n",now.sec(),now.usec());
  DSTRM_EVENT(MAIN_GROUP_FAM, START,0,0,NULL);

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

      Supplier_EC supplier_ec;
      if (supplier_ec.init(sched_type.c_str(), poa.in(),orb->orb_core()->reactor()) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Kokyu_EC"), 1);
        }

      for(size_t i=0; i<ior_input_files.size(); ++i)
        {
          Filename_Array::TYPE filename = ior_input_files[i];
          supplier_ec.init_gateway(orb.in(),
                                   poa.in(),
                                   filename
                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // ****************************************************************
      RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec_ior =
        supplier_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(supplier_ec_ior.in()
                                                    ACE_ENV_ARG_PARAMETER);

      ACE_OS::fprintf(ior_output_file, ior.in());
      ACE_OS::fclose(ior_output_file);


      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

      supplier_ec.start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      //DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t WORKER_ACTIVATED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 0, 0, NULL);
      //DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, m_id, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t WORKER_STARTED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, 0, 0, NULL);

#ifdef ACE_HAS_DSUI
      EC_Event_Limit* e_limit = new EC_Event_Limit (TAO_ORB_Core_instance(), ds_cntl);
#else
      EC_Event_Limit* e_limit = new EC_Event_Limit (TAO_ORB_Core_instance());
#endif //ACE_HAS_DSUI
      ACE_Time_Value ticker (120);
      //orb->orb_core()->reactor()->schedule_timer(e_limit,0, ticker);
      long timer_id = rt.reactor()->schedule_timer(e_limit,0,ticker);
      if (timer_id < 0)
        {
          ACE_DEBUG((LM_DEBUG,"Supplier_EC (%t) could not schedule EC_Event_Limit timer\n"));
        }

      rt.activate(); //need thread creation flags? or priority?
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      //@BT: ORB shutting down; currently, this isn't expected to happen
      //DSTRM_EVENT (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t CALL_SERVER_SHUTDOWN at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 0, 0, NULL);

      //@BT: Scheduler shuts down with the EC and ORB
      //DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t SCHEDULER_SHUTDOWN at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 0, 0, NULL);

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

      //@BT: Done clean up
      //DSTRM_EVENT (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t AFTER_SERVER_SHUTDOWN at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 0, 0, NULL);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;

  //@BT
  //DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t STOP at %u\n",ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);

  return 0;
}

// ****************************************************************

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "cs:o:i:");
  int c;
  //these used for handline '-i':
  const char* input_file;
  size_t len;
  char *filename;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (ior_output_file == 0)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for writing: %p\n",
                               get_opts.opt_arg ()), -1);
          }
        break;
      case 'i':
        input_file = get_opts.opt_arg();
        len = ACE_OS::strlen("file://")+ACE_OS::strlen(input_file)+1;
        filename = new char[len];
        sprintf(filename,"file://%s",input_file);
        ACE_DEBUG((LM_DEBUG,"Adding consumer IOR %s\n",filename));
        ior_input_files.push_back(filename);
        break;
      case 's':
        sched_type = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s -s <rms|muf|edf>"
                           " [-o iorfile]"
                           " [-i consumer_ec_ior]"
                           "\n"
                           "For multiple consumers, specify -i multiple times\n",
                           argv [0]),
                          -1);
      }
  if (ior_output_file == 0)
    {
      ior_output_file = ACE_OS::fopen ("supplier_ec.ior", "w");
    }
  if (ior_input_files.size() == 0)
    {
      ior_input_files.push_back("file://consumer_ec.ior");
    }
  // Indicates sucessful parsing of the command line
  return 0;
}
