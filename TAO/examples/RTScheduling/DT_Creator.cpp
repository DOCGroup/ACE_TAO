#include "DT_Creator.h"
#include "Thread_Task.h"
#include "Task_Stats.h"

#include "tao/ORB_Core.h"

#include <atomic>
#include <cerrno>
#include <new>
#include <utility>

#include "tao/RTScheduling/Current.h"

#include "ace/High_Res_Timer.h"
#include "ace/Lock_Adapter_T.h"

int
DT_Creator::dt_task_init (ACE_Arg_Shifter& arg_shifter)
{
  time_t start_time = 0;
  int load = 0;
  int iter = 0;
  int importance = 0;
  char *job_name {};
  int dist = 0;
  ACE_TCHAR const *current_arg {};

  if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Importance")) == 0)
    {
      arg_shifter.consume_arg ();
      current_arg = arg_shifter.get_current ();
      importance = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }

  if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Start_Time"))))
    {
      start_time = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }

  if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Iter"))))
    {
      iter = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }

  if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Load"))))
    {
      load = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }

  if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-JobName"))))
    {
      job_name = (char *)current_arg;
      dist = 1;
      arg_shifter.consume_arg ();
    }

  std::unique_ptr<Thread_Task> task =
    this->create_thr_task (importance,
                           start_time,
                           load,
                           iter,
                           dist,
                           job_name);
  if (!task)
  {
    return -1;
  }
  this->dt_list_.push_back (std::move (task));

  return 0;
}

int
DT_Creator::init (int argc, ACE_TCHAR *argv [])
{
  gsf_ = ACE_High_Res_Timer::global_scale_factor ();
  if (!this->state_lock_)
  {
    this->state_lock_ = std::make_unique<ACE_Lock_Adapter<TAO_SYNCH_MUTEX>> ();
  }
  if (!this->shutdown_lock_)
  {
    this->shutdown_lock_ = std::make_unique<ACE_Lock_Adapter<TAO_SYNCH_MUTEX>> ();
  }
  active_dt_count_ = 0;
  active_job_count_ = 0;
  this->log_.clear ();
  ACE_Arg_Shifter arg_shifter (argc, argv);

  ACE_TCHAR const *current_arg {};

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-GuidSeed"))))
        {
          TAO_RTScheduler_Current::guid_counter.store (
            ACE_OS::atoi (current_arg),
            std::memory_order_relaxed);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-DT_Count"))))
        {
          int const count = ACE_OS::atoi (current_arg);
          if (count < 0)
          {
            return -1;
          }
          this->dt_list_.reserve (static_cast<size_t> (count));
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-POA_Count"))))
        {
          int const count = ACE_OS::atoi (current_arg);
          if (count < 0)
          {
            return -1;
          }
          this->poa_list_.reserve (static_cast<size_t> (count));
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-JOB_Count"))))
        {
          int const count = ACE_OS::atoi (current_arg);
          if (count < 0)
          {
            return -1;
          }
          this->job_list_.reserve (static_cast<size_t> (count));
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-DT_Task")) == 0)
        {
          arg_shifter.consume_arg ();
          if (this->dt_task_init (arg_shifter) == -1)
          {
            return -1;
          }
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-POA")) == 0)
        {
          arg_shifter.consume_arg ();

          std::unique_ptr<POA_Holder> holder (
            new (std::nothrow) POA_Holder ());
          if (!holder)
            {
              errno = ENOMEM;
              return -1;
            }
          if (holder->init (arg_shifter) == -1)
            {
              return -1;
            }
          this->poa_list_.push_back (std::move (holder));
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Job")) == 0)
        {
          arg_shifter.consume_arg ();

          Job_i *job {};
          ACE_NEW_RETURN (job, Job_i (this), -1);
          PortableServer::ServantBase_var job_servant (job);
          if (job->init (arg_shifter) == -1)
          {
            return -1;
          }
          this->job_list_.push_back (job_servant);
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-OutFile"))))
        {
          file_name_ = current_arg;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-LogFile"))))
        {
          log_file_name_ = current_arg;
          arg_shifter.consume_arg ();
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }

  return 0;
}

void
DT_Creator::register_synch_obj ()
{
  CosNaming::Name name (1);
  name.length (1);

  CosNaming::NamingContext_var synch_context;

  try
    {
      // Try binding the sender context in the NS
      name [0].id =
        CORBA::string_dup ("Synch");

      synch_context = this->naming_->bind_new_context (name);

      //
      // We reach here if there was no exception raised in
      // <bind_new_context>.  We then create a receiver context.
      //

    }
  catch (const CosNaming::NamingContext::AlreadyBound& )
    {
      //
      // The synch context already exists, probably created by the
      // receiver(s).
      //

      // Get the synch context.
      name [0].id =
        CORBA::string_dup ("Synch");

      CORBA::Object_var object =
        this->naming_->resolve (name);

      synch_context = CosNaming::NamingContext::_narrow (object.in ());
    }

  ACE_CString synch_name ("Synch");
  ACE_Time_Value timestamp = ACE_OS::gettimeofday ();

  char buf [BUFSIZ];
  ACE_OS::sprintf(buf, "%lu", static_cast<unsigned long> (timestamp.sec ()));
  synch_name += buf;

  name [0].id =
    CORBA::string_dup (synch_name.c_str ());

  ACE_DEBUG ((LM_DEBUG,
        "Synch Name %C\n\n",
        synch_name.c_str ()));

  ACE_NEW (synch_,
     Synch_i);

  Synch_var synch = synch_->_this ();

  // Register the synch object with the Synch context.
  synch_context->rebind (name,
       synch.in ());
}

int
DT_Creator::activate_root_poa ()
{
  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (object.in ());

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  poa_manager->activate ();

  return 0;
}

void
DT_Creator::activate_poa_list ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
    "DT_Creator::activate_poa_list\n"));

  if (!this->poa_list_.empty ())
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ());

      object = this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      for (std::unique_ptr<POA_Holder> const & poa : this->poa_list_)
        {
          poa->activate (rt_orb.in(), root_poa.in ());
        }
    }
}

void
DT_Creator::activate_job_list ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
    "DT_Creator::activate_job_list\n"));

  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (object.in ());

  for (PortableServer::ServantBase_var const & job_servant : this->job_list_)
    {
      ++active_job_count_;

      Job_i * const job = dynamic_cast<Job_i *> (job_servant.in ());

      if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Activating job:%C\n", job->name ().c_str ()));
      }

      // find your poa
      PortableServer::POA_var host_poa =
        root_poa->find_POA (job->poa ().c_str (), false);

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
    } /* while */

  ACE_DEBUG ((LM_DEBUG,
              "Activated Job List\n"));
}

void
DT_Creator::activate_schedule ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
    "Activating schedule, task count = %B\n",
    this->dt_list_.size ()));

  for (std::unique_ptr<Thread_Task> const & task_owner : this->dt_list_)
    {
      Thread_Task * const task = task_owner.get ();

      if (task->dist ())
  {
    // resolve the object from the naming service
    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup (task->job ());

    CORBA::Object_var obj =
      this->naming_->resolve (name);

    Job_var job = Job::_narrow (obj.in ());

    //    if (TAO_debug_level > 0)
    // {
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
                  /*
      RTCORBA::PriorityModel priority_model =
        priority_policy->priority_model ();

      if (priority_model == RTCORBA::CLIENT_PROPAGATED)
        ACE_DEBUG ((LM_DEBUG,
        "%C priority_model = RTCORBA::CLIENT_PROPAGATED\n", task->job ()));
      else
        ACE_DEBUG ((LM_DEBUG,
        "%C priority_model = RTCORBA::SERVER_DECLARED\n", task->job ()));
      */
    }
        //} /*  if (TAO_debug_level > 0) */

    task->job (job.in ());
  }
    }
  if (TAO_debug_level > 0 && !this->dt_list_.empty ())
    ACE_DEBUG ((LM_DEBUG,
    "Activated schedule, task count = %B\n",
    this->dt_list_.size ()));
}

int
DT_Creator::resolve_naming_service ()
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
DT_Creator::create_distributable_threads (RTScheduling::Current_ptr current)
{
  long flags;
  flags = THR_NEW_LWP | THR_JOINABLE;
  flags |=
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();

  ACE_DEBUG ((LM_DEBUG,
              "Waiting to Synch\n"));

  while (!this->synch ()->synched ())
    {
      try
        {
          this->orb_->perform_work ();
        }
      catch (const CORBA::Exception &)
        {
          return;
        }
    }

  CORBA::Policy_var sched_param;
  sched_param = this->sched_param (100);
  char const *name {};
  current->begin_scheduling_segment (name,
              sched_param.in (),
              sched_param.in ());

  this->base_time_ = std::make_unique<ACE_Time_Value> (
    *this->synch ()->base_time ());

  for (std::unique_ptr<Thread_Task> const & task_owner : this->dt_list_)
    {
      Thread_Task * const task = task_owner.get ();
      ACE_Time_Value now (ACE_OS::gettimeofday ());

      ACE_Time_Value elapsed_time =  now - *base_time_;

      char buf [BUFSIZ];
      ACE_OS::sprintf (buf, "elapsed time = %d\n now = %d\n base_time_ = %d\n",
           (int) elapsed_time.sec (),
           (int) now.sec (),
           (int) base_time_->sec());

      this->log_msg (buf);

      ACE_hthread_t curr_thr;
      ACE_Thread::self (curr_thr);

      if (task->start_time () != 0 &&
          (elapsed_time.sec () < task->start_time ()))
        {
          time_t suspension_time = task->start_time () - elapsed_time.sec ();
          ACE_OS::sprintf (buf, "suspension_time = %lu\n",
                           static_cast<unsigned long> (suspension_time));
          this->log_msg (buf);
          yield (suspension_time, task);
        }

      sched_param = this->sched_param (task->importance ());
      task->activate_task (current,
                           sched_param.in (),
                           flags,
                           this->base_time_.get ());

      ++active_dt_count_;
    }

  this->wait ();

  current->end_scheduling_segment (name);

  this->check_ifexit ();
}

void
DT_Creator::dt_ended ()
{
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_dt_count_;
    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Active dt count = %d\n",active_dt_count_));
    }
    char buf [BUFSIZ];
    ACE_OS::sprintf (buf,"Active dt count = %d\n",active_dt_count_);
    this->log_.emplace_back (buf);
  }
  this->check_ifexit ();
}

void
DT_Creator::job_ended ()
{
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_job_count_;
    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Active job count = %d\n",active_job_count_));
    }
    char buf [BUFSIZ];
    ACE_OS::sprintf (buf,"Active job count = %d\n",active_job_count_);
    this->log_.emplace_back (buf);
  }

 this->check_ifexit ();
}

void
DT_Creator::check_ifexit ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
    "Checking exit status Job# = %d DT# = %d\n",
    active_job_count_,
    active_dt_count_));

  static int shutdown = 0;

  {
    ACE_GUARD (ACE_Lock, ace_mon, *shutdown_lock_);

    if (!shutdown)
      {
  // All tasks have finished and all jobs have been shutdown.
  if (active_dt_count_ == 0 && active_job_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Shutdown in progress ...\n"));

      /*
      for (std::unique_ptr<Thread_Task> const & task : this->dt_list_)
        {
          task->dump_stats ();
        }

      for (PortableServer::ServantBase_var const & servant : this->job_list_)
        {
          dynamic_cast<Job_i *> (servant.in ())->dump_stats ();
        }
      */
      TASK_STATS::instance ()->dump_samples (file_name_.c_str (),
               ACE_TEXT("#Schedule Output"));

      shutdown = 1;

      FILE* log_file = ACE_OS::fopen (log_file_name_.c_str (), "w");

      if (log_file != 0)
        {
          // First dump what the caller has to say.
          ACE_OS::fprintf (log_file, "Log File\n");

          {
            ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
            for (std::string const & message : this->log_)
              ACE_OS::fprintf (log_file, "%s\n", message.c_str ());
          }

          ACE_OS::fclose (log_file);
        }
      ACE_DEBUG ((LM_DEBUG,
      "Log File Ready\n"));
    }
      }
  }
}

int
DT_Creator::dt_count ()
{
  return static_cast<int> (this->dt_list_.size ());
}

DT_Creator::DT_Creator ()
 : active_dt_count_ (0),
   active_job_count_ (0),
   file_name_ (),
   log_file_name_ (),
   gsf_ (0),
   synch_ (0)
{
}

DT_Creator::~DT_Creator ()
{
  // Release the holders and servants while the ORB reference is still alive.
  this->dt_list_.clear ();
  this->poa_list_.clear ();
  this->job_list_.clear ();
}

void
DT_Creator::log_msg (std::string msg)
{
  ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
  this->log_.push_back (std::move (msg));
}

CORBA::ORB_ptr
DT_Creator::orb ()
{
  return this->orb_.in ();
}

void
DT_Creator::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
}

ACE_Time_Value*
DT_Creator::base_time ()
{
  return this->base_time_.get ();
}

void
DT_Creator::base_time (ACE_Time_Value* base_time)
{
  if (this->base_time_.get () != base_time)
  {
    this->base_time_.reset (base_time);
  }
}

Synch_i*
DT_Creator::synch ()
{
  return this->synch_.in ();
}
