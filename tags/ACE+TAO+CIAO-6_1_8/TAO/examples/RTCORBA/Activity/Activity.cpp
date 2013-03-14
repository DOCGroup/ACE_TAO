//$Id$
#include "Activity.h"
#include "Thread_Task.h"
#include "Job_i.h"
#include "POA_Holder.h"
#include "Builder.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "ace/High_Res_Timer.h"
#include "ace/Barrier.h"
#include "ace/Timer_Heap.h"
#include "ace/Service_Config.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Signal.h"
#include "ace/Reactor.h"


//***************************************************************//
extern "C" void handler (int)
{
  ACE_Service_Config::reconfig_occurred (1);
}

//***************************************************************//

Activity::Activity (void)
  :builder_ (0),
   barrier_ (0),
   active_task_count_ (0),
   active_job_count_ (0)
{
  state_lock_ = new ACE_Lock_Adapter <TAO_SYNCH_MUTEX>;
}

Activity::~Activity (void)
{
  delete state_lock_;
  delete barrier_;
}

void
Activity::builder (Builder* builder)
{
  builder_ = builder;
}

CORBA::ORB_ptr
Activity::orb (void)
{
  return orb_.in ();
}

RTCORBA::Current_ptr
Activity::current (void)
{
  return current_.in ();
}

int
Activity::init (int& argc, ACE_TCHAR *argv [])
{
  // Copy command line parameter.
  ACE_Argv_Type_Converter command_line(argc, argv);

  this->orb_ = CORBA::ORB_init (command_line.get_argc(),
                                command_line.get_TCHAR_argv());

  CORBA::Object_var object =
    orb_->resolve_initial_references ("RootPOA");

  root_poa_ =
    PortableServer::POA::_narrow (object.in ());

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager ();

  object =
    orb_->resolve_initial_references ("RTORB");

  this->rt_orb_ =
    RTCORBA::RTORB::_narrow (object.in ());

  object =
    orb_->resolve_initial_references ("RTCurrent");

  current_ =
    RTCORBA::Current::_narrow (object.in ());

  poa_manager->activate ();

  object = this->orb_->resolve_initial_references ("PriorityMappingManager");
  RTCORBA::PriorityMappingManager_var mapping_manager =
    RTCORBA::PriorityMappingManager::_narrow (object.in ());

  this->priority_mapping_ = mapping_manager->mapping ();

  return 0;
}

int
Activity::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Naming Service.\n"),
                      -1);

  this->naming_ =
    CosNaming::NamingContextExt::_narrow (naming_obj.in ());

  return 0;
}

void
Activity::activate_poa_list (void)
{
  POA_LIST list;
  int count = builder_->poa_list (list);

  for (int i = 0; i < count; ++i)
    {
      list[i]->activate (this->rt_orb_.in(), this->root_poa_.in ());
    }
}

void
Activity::activate_job_list (void)
{
  JOB_LIST list;
  int count = builder_->job_list (list);
  Job_i* job;

  for (int i = 0; i < count; ++i)
    {
      job = list[i];

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Activating job:%C\n", job->name ().c_str ()));

      // find your poa
      PortableServer::POA_var host_poa =
        root_poa_->find_POA (job->poa ().c_str (), 0);

      PortableServer::ServantBase_var servant_var (job);

      // Register with poa.
      PortableServer::ObjectId_var id;

      id = host_poa->activate_object (job);

      CORBA::Object_var server =
        host_poa->id_to_reference (id.in ());

      CORBA::String_var ior =
        orb_->object_to_string (server.in ());

      const ACE_CString &job_name = job->name ();

      CosNaming::Name_var name =
        this->naming_->to_name (job_name.c_str ());

      this->naming_->rebind (name.in (),
                             server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Registered %C with the naming service\n",
                  job_name.c_str ()));

      active_job_count_++;

    } /* while */
}

void
Activity::activate_schedule (void)
{
  TASK_LIST list;
  int count = builder_->task_list (list);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Activating schedule, task count = %d\n",
                count));

  ACE_NEW (barrier_, ACE_Barrier (count+1));

  Periodic_Task* task;

  for (int i = 0; i < count; ++i)
    {
      task = list[i];

      // resolve the object from the naming service
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (task->job ());

      CORBA::Object_var obj =
        this->naming_->resolve (name);

      Job_var job = Job::_narrow (obj.in ());

      if (TAO_debug_level > 0)
        {
          // Check that the object is configured with some
          // PriorityModelPolicy.
          CORBA::Policy_var policy =
            job->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

          RTCORBA::PriorityModelPolicy_var priority_policy =
            RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

          if (CORBA::is_nil (priority_policy.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "ERROR: Priority Model Policy not exposed!\n"));
          else
            {
              RTCORBA::PriorityModel priority_model =
                priority_policy->priority_model ();

              if (priority_model == RTCORBA::CLIENT_PROPAGATED)
                ACE_DEBUG ((LM_DEBUG,
                            "%C priority_model = RTCORBA::CLIENT_PROPAGATED\n", task->job ()));
              else
                ACE_DEBUG ((LM_DEBUG,
                            "%C priority_model = RTCORBA::SERVER_DECLARED\n", task->job ()));
            }
        } /*  if (TAO_debug_level > 0) */

      task->job (job.in ());
      task->activate_task (this->barrier_, this->priority_mapping_);
      active_task_count_++;

      ACE_DEBUG ((LM_DEBUG, "Job %C scheduled\n", task->job ()));
    }

  ACE_DEBUG ((LM_DEBUG, "(%P,%t) Waiting for tasks to synch...\n"));
  barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%P,%t) Tasks have synched...\n"));
}

void
Activity::task_ended (Periodic_Task* /*ended_task*/)
{
  ACE_DEBUG ((LM_DEBUG, "Active task count = %d\n",active_task_count_));
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_task_count_;
  }

  this->check_ifexit ();
}

void
Activity::job_ended (Job_i* /*ended_job*/)
{
  ACE_DEBUG ((LM_DEBUG, "Active job count = %d\n",active_job_count_));
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_job_count_;
  }

  this->check_ifexit ();
}

void
Activity::check_ifexit (void)
{
  // All tasks have finished and all jobs have been shutdown.
  if (active_task_count_ == 0 && active_job_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Shutdown in progress ...\n"));
      // ask all tasks to dump stats.

      TASK_LIST task_list;
      int count = builder_->task_list (task_list);

      ACE_TCHAR msg[BUFSIZ];
      ACE_OS::sprintf (msg, ACE_TEXT("# Stats generated on --\n"));

      for (int i = 0; i < count; ++i)
        {
          task_list[i]->dump_stats (msg);
        }

      // shutdown the ORB
      orb_->shutdown (0);
    }
}

CORBA::Short
Activity::get_server_priority (CORBA::Object_ptr server)
{
  // Get the Priority Model Policy from the stub.
  CORBA::Policy_var policy =
    server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

  // Narrow down to correct type.
  RTCORBA::PriorityModelPolicy_var priority_policy =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

  // Make sure that we have the SERVER_DECLARED priority model.
  RTCORBA::PriorityModel priority_model =
    priority_policy->priority_model ();
  if (priority_model != RTCORBA::SERVER_DECLARED)
    return -1;

  // Return the server priority.
  return priority_policy->server_priority ();
}

void
Activity::run (int argc, ACE_TCHAR *argv[])
{
  this->init (argc, argv);

  if (this->resolve_naming_service () == -1)
    return;

  this->activate_poa_list ();

  this->activate_job_list ();

  this->activate_schedule ();

  this->create_started_flag_file (argc, argv);

  orb_->run ();

  ACE_Thread_Manager::instance ()->wait ();

  CORBA::release (this->naming_);

  // Hack for proper cleanup.
  this->builder_->fini ();

  orb_->destroy ();
}

void
Activity::create_started_flag_file (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR* current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Started_Flag"))))
        {
          FILE *file = ACE_OS::fopen (current_arg, ACE_TEXT("w"));

          if (file == 0)
            ACE_ERROR ((LM_ERROR,
                        "Unable to open %s for writing: %p\n",
                        current_arg));

          ACE_OS::fprintf (file, "ignore");

          ACE_OS::fclose (file);

          arg_shifter.consume_arg ();
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_Builder);

  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGHUP);

  ACE_Timer_Heap timer_queue;
  ACE_Reactor::instance ()->timer_queue (&timer_queue);

  int rc = 0;
  try
    {
      ACTIVITY::instance()->run (argc, argv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      rc = 1;
    }

  // reset stack based timer queue
  ACE_Reactor::instance ()->timer_queue (0);

  return rc;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<Activity, ACE_Null_Mutex> *ACE_Singleton<Activity, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
