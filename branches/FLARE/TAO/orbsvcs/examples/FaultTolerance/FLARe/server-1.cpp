// $Id$

#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/Task.h"
#include "ace/Sample_History.h"
#include "ace/Throughput_Stats.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "LWFTS.h"
#include "tests/RTCORBA/common_args.cpp"
#include "tests/RTCORBA/check_supported_priorities.cpp"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "AppSideReg.h"
#include "AppOptions.h"
#include "ace/Barrier.h"
#include "ServerORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_stdio.h"
#include <fstream>
#include <sstream>

ACE_RCSID(Thread_Pools, server, "$Id$")

static int stop = 0;

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void method (CORBA::ULong start, CORBA::ULong end, CORBA::ULong work,
               CORBA::ULong prime_number, CORBA::ULong kill);

  void shutdown (void);

  PortableServer::POA_ptr _default_POA (void);

  void dump (void);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  ACE_Sample_History history_;
  ACE_hrtime_t start_;
  ACE_hrtime_t end_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    history_ (50)
{
}

void
test_i::method (CORBA::ULong test_start, CORBA::ULong test_end,
                CORBA::ULong work,
                CORBA::ULong prime_number,
                CORBA::ULong kill)
{
  /*
  CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("RTCurrent");

  RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (obj.in ());

  if (CORBA::is_nil (obj.in ()))
      throw CORBA::INTERNAL ();

  CORBA::Short servant_thread_priority =
    current->the_priority ();

  ACE_DEBUG ((LM_DEBUG,"Servant thread priority: %d\n",
             servant_thread_priority));
  */

  static int i = 0;
  ACE_DEBUG ((LM_DEBUG, "%d\n",i++));

  if (kill && stop)
    ACE_OS::exit (1);
  if (test_start == 1)
    {
      this->start_ = ACE_OS::gethrtime ();
    }
  ACE_hrtime_t start = ACE_OS::gethrtime ();
  
  for (; work != 0; work--)
    ACE::is_prime (prime_number,
                   2,
                   prime_number / 2);
  ACE_hrtime_t end = ACE_OS::gethrtime ();

  // ACE_DEBUG ((LM_DEBUG, "Time taken = %d\n", end - start));
  this->history_.sample (end - start);
  if (test_end == 1)
    {
      this->end_ = ACE_OS::gethrtime ();
    }
}

void
test_i::dump (void)
{
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_Basic_Stats stats;
  this->history_.collect_basic_stats (stats);
  stats.dump_results ("Total", gsf);
  ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                         this->end_ - this->start_,
                                         stats.samples_count ());
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
/*
static const char *ior_output_file_1 = "s1.ior";
static const char *ior_output_file_2 = "s2.ior";
static const char *ior_output_file_3 = "s3.ior";
static const char *ior_output_file_4 = "s4.ior";
*/
static CORBA::ULong static_threads = 1;
static CORBA::ULong dynamic_threads = 0;
static CORBA::ULong number_of_lanes = 0;
static RTCORBA::Priority default_thread_priority = 0;
static RTCORBA::Priority pool_priority = ACE_INT16_MIN;

static const char *bands_file = "empty-file";
static const char *lanes_file = "empty-file";
std::string first_object_id;
//std::string second_object_id;
size_t first_object_role;
double first_object_load;
//size_t second_object_role;
const char *rm_ior_file = "file://rm.ior";

void
read_object_info (std::string file_name)
{
  std::ifstream input_file (file_name.c_str ());
  input_file >> first_object_id;
  input_file >> first_object_role;
  input_file >> first_object_load;
//  input_file >> second_object_id;
//  input_file >> second_object_role;
}

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv,
                        "b:f:hl:n:s:a:" // server options
                        "c:e:g:hi:j:k:m:p:q:r:t:u:v:w:x:y:z:" // client options
                        );
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        bands_file = get_opts.opt_arg ();
        break;

      case 'f':
        pool_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        lanes_file = get_opts.opt_arg ();
        break;

      case 'n':
        number_of_lanes = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        static_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
     
      case 'a':
        stop = ACE_OS::atoi (get_opts.opt_arg());
        break;

        
      case 'c':
      case 'd':
      case 'e':
      case 'g':
      case 'i':
      case 'j':
      case 'k':
      case 'm':
      case 'p':
      case 'q':
      case 'r':
      case 't':
      case 'u':
      case 'v':
      case 'w':
      case 'x':
      case 'y':
      case 'z':
        // client options: ignored.
        break;
/*
      case 'h':
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-b <bands file> (defaults to %s)\n"
                           "\t-f <pool priority> (defaults to %d)\n"
                           "\t-h <help: shows options menu>\n"
                           "\t-l <lanes file> (defaults to %s)\n"
                           "\t-n <number of lanes> (defaults to %d)\n"
                           "\t-o <ior file> (defaults to %s)\n"
                           "\t-s <static threads> (defaults to %d)\n"
                           "\n",
                           argv [0],
                           bands_file,
                           default_thread_priority,
                           lanes_file,
                           number_of_lanes,
                           ior_output_file_1,
                           static_threads),
                          -1);
*/
      }

  return 0;
}

int
write_ior_to_file (const char *ior_file,
                   CORBA::ORB_ptr orb,
                   CORBA::Object_ptr object)
{
  CORBA::String_var ior =
    orb->object_to_string (object);

  FILE *output_file =
    ACE_OS::fopen (ior_file,
                   "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       ior_file),
                      -1);

  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());

  ACE_OS::fclose (output_file);

  return 0;
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      default_thread_priority =
        current->the_priority ();

      int result = 0;
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      CORBA::PolicyList policies;

      CORBA::Boolean allow_borrowing = 0;
      if (number_of_lanes != 0)
        {
          get_auto_priority_lanes_and_bands (number_of_lanes,
                                             rt_orb.in (),
                                             stacksize,
                                             static_threads,
                                             dynamic_threads,
                                             allow_request_buffering,
                                             max_buffered_requests,
                                             max_request_buffer_size,
                                             allow_borrowing,
                                             policies,
                                             1);
        }
      else if (ACE_OS::strcmp (lanes_file, "empty-file") != 0)
        {
          result =
            get_priority_lanes ("server",
                                lanes_file,
                                rt_orb.in (),
                                stacksize,
                                static_threads,
                                dynamic_threads,
                                allow_request_buffering,
                                max_buffered_requests,
                                max_request_buffer_size,
                                allow_borrowing,
                                policies,
                                1);

          if (result != 0)
            return result;

          result =
            get_priority_bands ("server",
                                bands_file,
                                rt_orb.in (),
                                policies,
                                1);

          if (result != 0)
            return result;
        }
      else
        {
          if (pool_priority == ACE_INT16_MIN)
            pool_priority =
              default_thread_priority;

          RTCORBA::ThreadpoolId threadpool_id =
            rt_orb->create_threadpool (stacksize,
                                       static_threads,
                                       dynamic_threads,
                                       pool_priority,
                                       allow_request_buffering,
                                       max_buffered_requests,
                                       max_request_buffer_size);

          policies.length (policies.length () + 1);
          policies[policies.length () - 1] =
            rt_orb->create_threadpool_policy (threadpool_id);

          if (ACE_OS::strcmp (bands_file, "empty-file") != 0)
            {
              result =
                get_priority_bands ("server",
                                    bands_file,
                                    rt_orb.in (),
                                    policies,
                                    1);

              if (result != 0)
                return result;
            }
        }

      /*
      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_implicit_activation_policy
        (PortableServer::IMPLICIT_ACTIVATION);
      */

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              default_thread_priority);

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

      policies.length (policies.length () + 1);
      policies[policies.length () - 1] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var poa =
        root_poa->create_POA ("RT POA",
                              poa_manager.in (),
                              policies);

      read_object_info (AppOptions::instance ()->object_info_file ());

      ACE_DEBUG ((LM_DEBUG, "Getting RM\n"));

      CORBA::Object_var tmp = this->orb_->string_to_object (rm_ior_file);
      ReplicationManager_var rm =
        ReplicationManager::_narrow (tmp.in ());

      // ***************************************************
      // first servant activated

      test_i *first_servant =
        new test_i (this->orb_.in (),
                    poa.in ());

      PortableServer::ServantBase_var first_safe_servant 
        (first_servant);
      ACE_UNUSED_ARG (first_safe_servant);

      /*
      test_var first_test =
        first_servant->_this ();
      */

      PortableServer::ObjectId_var id_1 =
        PortableServer::string_to_ObjectId (first_object_id.c_str ());
      poa->activate_object_with_id (id_1.in (), first_servant);
      CORBA::Object_var first_servant_object =
        poa->id_to_reference (id_1.in ());
      test_var first_test = test::_narrow (first_servant_object.in ());

      std::ostringstream ostr;
      ostr << first_object_id  << first_object_role << ".ior";
      result =
        write_ior_to_file (ostr.str().c_str(),
                           this->orb_.in (),
                           first_test.in ());

      rm->register_application (first_object_id.c_str (), first_object_load,
        AppOptions::instance ()->host_id ().c_str (),
        AppOptions::instance ()->process_id ().c_str (),
        first_object_role,
        first_test.in ());

      if (result != 0)
        return result;

      // ***************************************************

      // ***************************************************
      // second servant activated
/*
      ACE_DEBUG ((LM_DEBUG, "activating second object\n"));

      test_i *second_servant =
        new test_i (this->orb_.in (),
                    poa.in ());

      PortableServer::ServantBase_var second_safe_servant 
        (second_servant);
      ACE_UNUSED_ARG (second_safe_servant);

      
      test_var second_test =
        second_servant->_this ();
      

      PortableServer::ObjectId_var id_2 =
        PortableServer::string_to_ObjectId (second_object_id.c_str ());
      poa->activate_object_with_id (id_2.in (), second_servant);
      CORBA::Object_var second_servant_object =
        poa->id_to_reference (id_2.in ());
      test_var second_test = test::_narrow (second_servant_object.in ());

      result =
        write_ior_to_file (ior_output_file_2,
                           this->orb_.in (),
                           second_test.in ());

      rm->register_application (second_object_id.c_str (), 
        AppOptions::instance ()->host_id ().c_str (),
        AppOptions::instance ()->process_id ().c_str (),
        second_object_role,
        second_test.in ());

      if (result != 0)
        return result;
*/
      // ***************************************************

      poa_manager->activate ();

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      AppOptions::instance ()->parse_args (argc, argv);
      
      PortableInterceptor::ORBInitializer_ptr tmp;

      ACE_NEW_RETURN (tmp,
                      ServerORBInitializer,
                      -1); // No CORBA exceptions yet!

      PortableInterceptor::ORBInitializer_var orb_initializer = tmp;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "");
      
      ACE_Barrier thread_barrier (2);
      AppSideReg proc_reg (&thread_barrier, orb.in());
      proc_reg.activate ();
      thread_barrier.wait();

      ACE_DEBUG ((LM_DEBUG, "After initialization of AppSide\n"));


      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in ());

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
