//$Id$
#include "Activity.h"

#include "ace/High_Res_Timer.h"
#include "ace/Synch.h"
#include "ace/Timer_Heap.h"
#include "ace/Service_Config.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "tao/ORB_Core.h"

#include "Thread_Task.h"
#include "Job_i.h"
#include "POA_Holder.h"
#include "Builder.h"

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
Activity::init (int& argc, char *argv []
                ACE_ENV_ARG_DECL)
{
  // Copy command line parameter.
  ACE_Argv_Type_Converter command_line(argc, argv);

  this->orb_ = CORBA::ORB_init (command_line.get_argc(),
                                command_line.get_ASCII_argv(),
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var object =
    orb_->resolve_initial_references ("RootPOA"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  root_poa_ =
    PortableServer::POA::_narrow (object.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  object =
    orb_->resolve_initial_references ("RTORB"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->rt_orb_ =
    RTCORBA::RTORB::_narrow (object.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  object =
    orb_->resolve_initial_references ("RTCurrent"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  current_ =
    RTCORBA::Current::_narrow (object.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  object = this->orb_->resolve_initial_references ("PriorityMappingManager"
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  RTCORBA::PriorityMappingManager_var mapping_manager =
    RTCORBA::PriorityMappingManager::_narrow (object.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->priority_mapping_ = mapping_manager->mapping ();

  return 0;
}

int
Activity::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Naming Service.\n"),
                      -1);

  this->naming_ =
    CosNaming::NamingContextExt::_narrow (naming_obj.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //@@tmp hack, otherwise crashes on exit!..??
  CosNaming::NamingContextExt::_duplicate (this->naming_.in());
  return 0;
}

void
Activity::activate_poa_list (ACE_ENV_SINGLE_ARG_DECL)
{
  POA_LIST list;
  int count = builder_->poa_list (list);

  for (int i = 0; i < count; ++i)
    {
      list[i]->activate (this->rt_orb_.in(), this->root_poa_.in ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Activity::activate_job_list (ACE_ENV_SINGLE_ARG_DECL)
{
  JOB_LIST list;
  int count = builder_->job_list (list);
  Job_i* job;

  for (int i = 0; i < count; ++i)
    {
      job = list[i];

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Activating job:%s\n", job->name ().c_str ()));

      // find your poa
      PortableServer::POA_var host_poa =
        root_poa_->find_POA (job->poa ().c_str (), 0
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      PortableServer::ServantBase_var servant_var (job);

      // Register with poa.
      PortableServer::ObjectId_var id;

      id = host_poa->activate_object (job
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Object_var server =
        host_poa->id_to_reference (id.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::String_var ior =
        orb_->object_to_string (server.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      const ACE_CString &job_name = job->name ();

      CosNaming::Name_var name =
        this->naming_->to_name (job_name.c_str ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->naming_->rebind (name.in (),
                             server.in ()
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Registered %s with the naming service\n",
                  job_name.c_str ()));

      active_job_count_++;

    } /* while */
}

void
Activity::activate_schedule (ACE_ENV_SINGLE_ARG_DECL)
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
        this->naming_->resolve (name ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      Job_var job = Job::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (TAO_debug_level > 0)
        {
          // Check that the object is configured with some
          // PriorityModelPolicy.
          CORBA::Policy_var policy =
            job->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE
                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          RTCORBA::PriorityModelPolicy_var priority_policy =
            RTCORBA::PriorityModelPolicy::_narrow (policy.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (CORBA::is_nil (priority_policy.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "ERROR: Priority Model Policy not exposed!\n"));
          else
            {
              RTCORBA::PriorityModel priority_model =
                priority_policy->priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              if (priority_model == RTCORBA::CLIENT_PROPAGATED)
                ACE_DEBUG ((LM_DEBUG,
                            "%s priority_model = RTCORBA::CLIENT_PROPAGATED\n", task->job ()));
              else
                ACE_DEBUG ((LM_DEBUG,
                            "%s priority_model = RTCORBA::SERVER_DECLARED\n", task->job ()));
            }
        } /*  if (TAO_debug_level > 0) */

      task->job (job.in ());
      task->activate_task (this->barrier_, this->priority_mapping_);
      active_task_count_++;

      ACE_DEBUG ((LM_DEBUG, "Job %s scheduled\n", task->job ()));
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

      char msg[BUFSIZ];
      ACE_OS::sprintf (msg, "# Stats generated on --\n");

      for (int i = 0; i < count; ++i)
        {
          task_list[i]->dump_stats (msg);
        }

      // shutdown the ORB
      orb_->shutdown (0);
    }
}

CORBA::Short
Activity::get_server_priority (CORBA::Object_ptr server
                               ACE_ENV_ARG_DECL)
{
  // Get the Priority Model Policy from the stub.
  CORBA::Policy_var policy =
    server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow down to correct type.
  RTCORBA::PriorityModelPolicy_var priority_policy =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Make sure that we have the SERVER_DECLARED priority model.
  RTCORBA::PriorityModel priority_model =
    priority_policy->priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (priority_model != RTCORBA::SERVER_DECLARED)
    return -1;

  // Return the server priority.
  return priority_policy->server_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Activity::run (int argc, char *argv[] ACE_ENV_ARG_DECL)
{
  this->init (argc, argv ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER) == -1)
    return;
  ACE_CHECK;

  this->activate_poa_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->activate_job_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->activate_schedule (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->create_started_flag_file (argc, argv);

  orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  orb_->destroy ();

  ACE_Thread_Manager::instance ()->wait ();
}

void
Activity::create_started_flag_file (int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR* current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter (ACE_LIB_TEXT("-Started_Flag"))))
        {
          FILE *file = ACE_OS::fopen (current_arg, ACE_LIB_TEXT("w"));

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<Activity, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<Activity, ACE_Null_Mutex>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
