//$Id$
#include "Activity.h"

#include "ace/High_Res_Timer.h"
#include "ace/Synch.h"
#include "ace/Timer_Heap.h"
#include "ace/Service_Config.h"
#include "tao/ORB_Core.h"

#include "Thread_Task.h"
#include "Job_i.h"
#include "POA_Holder.h"
#include "Builder.h"

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

long
Activity::scope_policy (void)
{
  return thr_scope_policy_;
}

long
Activity::sched_policy (void)
{
  return thr_sched_policy_;
}

RTCORBA::Current_ptr
Activity::current (void)
{
  return current_.in ();
}

int
Activity::init (int& argc, char *argv []
                TAO_ENV_ARG_DECL)
{
  this->orb_ =
    CORBA::ORB_init (argc,
                     argv,
                     ""
                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->init_sched ();

  CORBA::Object_var object =
    orb_->resolve_initial_references ("RootPOA"
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  root_poa_ =
    PortableServer::POA::_narrow (object.in ()
                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  object =
    orb_->resolve_initial_references ("RTORB"
                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->rt_orb_ =
    RTCORBA::RTORB::_narrow (object.in ()
                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  object =
    orb_->resolve_initial_references ("RTCurrent"
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  current_ =
    RTCORBA::Current::_narrow (object.in ()
                               TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Activity::resolve_naming_service (TAO_ENV_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService"
                                            TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Naming Service.\n"),
                      -1);

  this->naming_ =
    CosNaming::NamingContextExt::_narrow (naming_obj.in ()
                                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //@@tmp hack, otherwise crashes on exit!..??
  CosNaming::NamingContextExt::_duplicate (this->naming_.in());
  return 0;
}

void
Activity::activate_poa_list (TAO_ENV_SINGLE_ARG_DECL)
{
  POA_LIST list;
  int count = builder_->poa_list (list);

  for (int i = 0; i < count; ++i)
    {
      list[i]->activate (this->rt_orb_.in(), this->root_poa_.in ()
                         TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Activity::activate_job_list (TAO_ENV_SINGLE_ARG_DECL)
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
                             TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      PortableServer::ServantBase_var servant_var (job);

      // Register with poa.
      PortableServer::ObjectId_var id;

      id = host_poa->activate_object (job
                                      TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Object_var server =
        host_poa->id_to_reference (id.in ()
                                   TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::String_var ior =
        orb_->object_to_string (server.in ()
                                TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      const ACE_CString &job_name = job->name ();

      CosNaming::Name_var name =
        this->naming_->to_name (job_name.c_str ()
                                TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->naming_->rebind (name.in (),
                             server.in ()
                             TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Registered %s with the naming service\n",
                  job_name.c_str ()));

      active_job_count_++;

    } /* while */
}

void
Activity::activate_schedule (TAO_ENV_SINGLE_ARG_DECL)
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
                this->naming_->resolve (name TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      Job_var job = Job::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (TAO_debug_level > 0)
        {
          // Check that the object is configured with some
          // PriorityModelPolicy.
          CORBA::Policy_var policy =
            job->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE
                              TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;

          RTCORBA::PriorityModelPolicy_var priority_policy =
            RTCORBA::PriorityModelPolicy::_narrow (policy.in ()
                                                   TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (CORBA::is_nil (priority_policy.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "ERROR: Priority Model Policy not exposed!\n"));
          else
            {
              RTCORBA::PriorityModel priority_model =
                priority_policy->priority_model (TAO_ENV_SINGLE_ARG_PARAMETER);
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
      task->activate (barrier_);
      active_task_count_++;

      ACE_DEBUG ((LM_DEBUG, "Job %s scheduled\n", task->job ()));
    }

  ACE_DEBUG ((LM_DEBUG, "Waiting for tasks to synch..."));
  barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "Tasks have synched..."));
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
                               TAO_ENV_ARG_DECL)
{
  // Get the Priority Model Policy from the stub.
  CORBA::Policy_var policy =
    server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE
                         TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow down to correct type.
  RTCORBA::PriorityModelPolicy_var priority_policy =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Make sure that we have the SERVER_DECLARED priority model.
  RTCORBA::PriorityModel priority_model =
    priority_policy->priority_model (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (priority_model != RTCORBA::SERVER_DECLARED)
    return -1;

  // Return the server priority.
  return priority_policy->server_priority (TAO_ENV_SINGLE_ARG_PARAMETER);
}

int
Activity::init_sched (void)
{
  thr_sched_policy_ = orb_->orb_core ()->orb_params ()->sched_policy ();
  thr_scope_policy_ = orb_->orb_core ()->orb_params ()->scope_policy ();

  if (thr_sched_policy_ == THR_SCHED_FIFO)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_FIFO\n"));

      sched_policy_ = ACE_SCHED_FIFO;
    }
  else if (thr_sched_policy_ == THR_SCHED_RR)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_RR\n"));

      sched_policy_ = ACE_SCHED_RR;
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Sched policy = THR_SCHED_OTHER\n"));

      sched_policy_ = ACE_SCHED_OTHER;
    }

  int min_priority = ACE_Sched_Params::priority_min (sched_policy_);
  int max_priority = ACE_Sched_Params::priority_max (sched_policy_);

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "max_priority = %d, min_priority = %d\n",
                  max_priority, min_priority));

      if (max_priority == min_priority)
        {
          ACE_DEBUG ((LM_DEBUG,"Detected max_priority == min_priority\n"));
        }
    }

  // Set the main thread to min priority...
  int priority = min_priority;

  if (ACE_OS::sched_params (ACE_Sched_Params (sched_policy_,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t): sched_params failed\n"),-1);
                           }

  return 0;
}

void
Activity::run (int argc, char *argv[] TAO_ENV_ARG_DECL)
{
  this->init (argc, argv TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->resolve_naming_service (TAO_ENV_SINGLE_ARG_PARAMETER) == -1)
          return;
  ACE_CHECK;

  this->activate_poa_list (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->activate_job_list (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

   this->activate_schedule (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  orb_->run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  orb_->destroy ();

  ACE_Thread_Manager::instance ()->wait ();
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_Builder);

  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGHUP);

  ACE_Timer_Heap timer_queue;
  ACE_Reactor::instance ()->set_timer_queue (&timer_queue);

  ACE_TRY_NEW_ENV
    {
      ACTIVITY::instance()->run (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<Activity, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<Activity, ACE_Null_Mutex>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
