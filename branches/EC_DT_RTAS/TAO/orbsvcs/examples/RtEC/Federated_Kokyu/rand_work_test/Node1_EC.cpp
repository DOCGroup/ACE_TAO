// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"
#include "ace/OS_NS_sys_time.h" //for ACE_OS::gettimeofday()
#include "ace/Select_Reactor_Base.h" //for ACE_Select_Reactor_Impl::DEFAULT_SIZE
#include "ace/Reactor.h"

#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "tao/ORB_Core.h"

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
  bool time_master = false;
  ACE_CString sched_type ="rms";
  FILE * ior_output_file;
  ACE_CString ior_output_filename = "node1_ec.ior";

  Gateway_Initializer::Filename_Array ior_input_files;
}

class Node1_EC : public Kokyu_EC
{
public:
  Node1_EC()
  {
  } //Node1_EC()

  ~Node1_EC(void)
  {
  } //~Node1_EC()

  void set_up_supp_and_cons (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ))
  {
    // P 1

    subtask_t subt;
    int supp_id,next_supp_id;

    // T_1_4 125 849 335
    subt.task_index = 1;
    subt.subtask_index = 4;
    subt.exec = ACE_Time_Value(0,125);
    subt.period = ACE_Time_Value(0,849);
    subt.phase = ACE_Time_Value(0,335);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_last_subtask(subt,supp_id //use same val for event type and supplier ids
                        ACE_ENV_ARG_PARAMETER);

    // T_4_4 23 257 132
    subt.task_index = 4;
    subt.subtask_index = 4;
    subt.exec = ACE_Time_Value(0,23);
    subt.period = ACE_Time_Value(0,257);
    subt.phase = ACE_Time_Value(0,132);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_last_subtask(subt,supp_id //use same val for event type and supplier ids
                        ACE_ENV_ARG_PARAMETER);

    // T_7_3 12 110 26
    subt.task_index = 7;
    subt.subtask_index = 3;
    subt.exec = ACE_Time_Value(0,12);
    subt.period = ACE_Time_Value(0,110);
    subt.phase = ACE_Time_Value(0,26);

    supp_id = 100*subt.task_index + subt.subtask_index;
    next_supp_id = 100*subt.task_index + subt.subtask_index+1;
    set_up_middle_subtask(subt,supp_id,next_supp_id,
                         supp_id,next_supp_id //use same val for event type and supplier ids
                         ACE_ENV_ARG_PARAMETER);

    // T_8_1 160 752 481
    subt.task_index = 8;
    subt.subtask_index = 1;
    subt.exec = ACE_Time_Value(0,160);
    subt.period = ACE_Time_Value(0,752);
    subt.phase = ACE_Time_Value(0,481);
    //subt.period = ACE_Time_Value(1,849);
    //subt.phase = ACE_Time_Value(5,335);

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
  TAO_EC_Kokyu_Factory::init_svcs ();
  TAO_EC_Gateway_IIOP_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
#ifdef ACE_HAS_DSUI
      ds_control ds_cntl("Node1_Test_Supplier","node1_enabled.dsui");
#endif //ACE_HAS_DSUI

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

      Node1_EC supplier_ec;
      if (supplier_ec.init(time_master,sched_type.c_str(), poa.in(),orb->orb_core()->reactor()) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Kokyu_EC"), 1);
        }

      RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec_ior =
        supplier_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(supplier_ec_ior.in()
                                                    ACE_ENV_ARG_PARAMETER);

      ACE_OS::fprintf(ior_output_file, ior.in());
      ACE_OS::fclose(ior_output_file);
      /*
      for(size_t i=0; i<ior_input_files.size(); ++i)
        {
          Gateway_Initializer::Filename_Array::TYPE filename = ior_input_files[i];
          supplier_ec.init_gateway(orb.in(),
                                   poa.in(),
                                   filename,
                                   ior_output_filename.c_str()
                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      */
      // ****************************************************************

      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

      supplier_ec.set_up_supp_and_cons(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      //supplier_ec.start(ACE_ENV_SINGLE_ARG_PARAMETER);
      //ACE_TRY_CHECK;

#ifdef ACE_HAS_DSUI
      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      //DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Node1_EC thread %t WORKER_ACTIVATED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 0, 0, NULL);
      //DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, m_id, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Node1_EC thread %t WORKER_STARTED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, 0, 0, NULL);
#endif //ACE_HAS_DSUI

      //schedule gateway inits for after we're sure all the processes have reached orb->run()
      Gateway_Initializer * ginit;
      ACE_NEW_RETURN(ginit,
                     Gateway_Initializer(),-1);
      ginit->init(orb,poa,&supplier_ec,ior_output_filename,ior_input_files);
      ACE_Time_Value gateway_delay(5,000000);
      long timer_id = rt.reactor()->schedule_timer(ginit,0,gateway_delay);
      if (timer_id < 0)
        {
          ACE_DEBUG((LM_DEBUG,"Node1_EC (%t) could not schedule Gateway_Initializer timer\n"));
        }

#ifdef ACE_HAS_DSUI
      //@BT
      //DSTRM_EVENT(MAIN_GROUP_FAM, START,1,0,NULL);
      ACE_Time_Value now(ACE_OS::gettimeofday());
      ACE_OS::printf("Node1_EC START at %isec %iusec\n",now.sec(),now.usec());
      DSTRM_EVENT(MAIN_GROUP_FAM, START,0,0,NULL);
#endif //ACE_HAS_DSUI

      rt.activate(); //need thread creation flags? or priority?
      ACE_Time_Value stop_time(340,0); //5min + allowance for gateway init and synchronous EC start
      orb->run (stop_time ACE_ENV_ARG_PARAMETER);
      //orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

#ifdef ACE_HAS_DSUI
      //@BT
      //DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Node1_EC thread %t STOP at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
#endif //ACE_HAS_DSUI

      // ****************************************************************

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Node1_EC");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// ****************************************************************

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ms:o:i:");
  int c;
  //these used for handline '-i':
  const char* input_file;
  size_t len;
  char *filename;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_filename = get_opts.opt_arg();
        ior_output_file = ACE_OS::fopen (ior_output_filename.c_str(), "w");
        if (ior_output_file == 0)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for writing: %p\n",
                               ior_output_filename.c_str()), -1);
          }
        break;
      case 'i':
        input_file = get_opts.opt_arg();
        len = ACE_OS::strlen(input_file)+1;
        filename = new char[len];
        sprintf(filename,"%s",input_file);
        ACE_DEBUG((LM_DEBUG,"Adding consumer IOR %s\n",filename));
        ior_input_files.push_back(filename);
        break;
      case 's':
        sched_type = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;
      case 'm':
        time_master = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s -s <rms|muf|edf>"
                           " [-o iorfile]"
                           " [-i consumer_ec_ior]"
                           " [-m]"
                           "\n"
                           "For multiple consumers, specify -i multiple times\n"
                           "If '-m' is present, this node will tell the others when to start\n",
                           argv [0]),
                          -1);
      }
  if (ior_output_file == 0)
    {
      ior_output_file = ACE_OS::fopen (ior_output_filename.c_str(), "w");
    }
  if (ior_input_files.size() == 0)
    {
      ior_input_files.push_back("node2_ec.ior");
    }
  // Indicates sucessful parsing of the command line
  return 0;
}
