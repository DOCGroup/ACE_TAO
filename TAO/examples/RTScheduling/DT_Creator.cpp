//$Id$


#include "DT_Creator.h"
#include "Thread_Task.h"
#include "tao/ORB_Core.h"
#include "Task_Stats.h"
#include "ace/High_Res_Timer.h"
#include "Task.h"
#include "DT_Creator.h"
#include "tao/RTScheduling/Current.h"

ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> guid_counter;

int
DT_Creator::dt_task_init (ACE_Arg_Shifter& arg_shifter)
{
  static int dt_index = 0;
  int start_time = 0;
  int load = 0;
  int iter = 0;
  int importance = 0;
  char *job_name = 0;
  int dist = 0;
  const ACE_TCHAR* current_arg = 0;
  if (arg_shifter.cur_arg_strncasecmp ("-Importance") == 0)
    {
      arg_shifter.consume_arg ();
      current_arg = arg_shifter.get_current ();
      importance = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }
  if ((current_arg = arg_shifter.get_the_parameter ("-Start_Time")))
    {
      start_time = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }
  if ((current_arg = arg_shifter.get_the_parameter ("-Iter")))
    {
      iter = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }
  if ((current_arg = arg_shifter.get_the_parameter ("-Load")))
    {
      load = ACE_OS::atoi (current_arg);
      arg_shifter.consume_arg ();
    }
  if ((current_arg = arg_shifter.get_the_parameter ("-JobName")))
    {
      job_name = (char *)current_arg;
      dist = 1;
      arg_shifter.consume_arg ();
    }
  dt_list_ [dt_index++] = this->create_thr_task (importance,
						 start_time,
						 load,
						 iter,
						 dist,
						 job_name);
  return 0;

}

int log_index = 0;
int
DT_Creator::init (int argc, char *argv [])
{
  gsf_ = ACE_High_Res_Timer::global_scale_factor ();
  state_lock_ = new ACE_Lock_Adapter <TAO_SYNCH_MUTEX>;
  shutdown_lock_ = new ACE_Lock_Adapter <TAO_SYNCH_MUTEX>;
  active_dt_count_ = 0;
  active_job_count_ = 0;
  ACE_NEW_RETURN (log, char*[BUFSIZ * 100],-1);

  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR* current_arg = 0;

  dt_count_ = 0;
  poa_count_ = 0;
  int poa_count = 0;
  job_count_ = 0;
  int job_count = 0;
  while (arg_shifter.is_anything_left ())
    {
	if ((current_arg = arg_shifter.get_the_parameter ("-GuidSeed")))
        {
	  guid_counter = (long) ACE_OS::atoi (current_arg);
	  arg_shifter.consume_arg ();
	}
	else if ((current_arg = arg_shifter.get_the_parameter ("-DT_Count")))
	  {
	    dt_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (dt_list_, Thread_Task*[dt_count_], -1);
	  active_dt_count_ = dt_count_;
          arg_shifter.consume_arg ();
	  }
      else if ((current_arg = arg_shifter.get_the_parameter ("-POA_Count")))
        {
          poa_count_ = ACE_OS::atoi (current_arg);
          ACE_NEW_RETURN (poa_list_, POA_Holder*[poa_count_], -1);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-JOB_Count")))
        {
          job_count_ = ACE_OS::atoi (current_arg);
	  active_job_count_ = job_count_;
	  ACE_NEW_RETURN (job_list_, Job_i*[job_count_], -1);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-DT_Task") == 0)
	{
          arg_shifter.consume_arg ();
	  dt_task_init (arg_shifter);
	}
      else if (arg_shifter.cur_arg_strncasecmp ("-POA") == 0)
	{
          arg_shifter.consume_arg ();

          ACE_NEW_RETURN (this->poa_list_[poa_count], POA_Holder (), -1);

          if (this->poa_list_[poa_count]->init (arg_shifter) == -1)
            {
              delete this->poa_list_[poa_count];
              return -1;
            }
	  else
	    poa_count++;
	}
      else if (arg_shifter.cur_arg_strncasecmp ("-Job") == 0)
	{
          arg_shifter.consume_arg ();

          ACE_NEW_RETURN (this->job_list_[job_count], Job_i (this), -1);

          if (this->job_list_[job_count]->init (arg_shifter) == -1)
	    {
	      delete this->job_list_[job_count];
	      return -1;
	    }
	  else 
	    job_count++;
	}
      else if ((current_arg = arg_shifter.get_the_parameter ("-OutFile")))
	{
	  file_name_ = CORBA::string_dup (current_arg);
	  arg_shifter.consume_arg ();
	}
      else if ((current_arg = arg_shifter.get_the_parameter ("-LogFile")))
	{
	  log_file_name_ = CORBA::string_dup (current_arg);
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
DT_Creator::register_synch_obj (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNaming::Name name (1);
  name.length (1);

  CosNaming::NamingContext_var synch_context;

  ACE_TRY
    {
      // Try binding the sender context in the NS
      name [0].id =
        CORBA::string_dup ("Synch");

      synch_context = this->naming_->bind_new_context (name
						       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //
      // We reach here if there was no exception raised in
      // <bind_new_context>.  We then create a receiver context.
      //

    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, al_ex)
    {
      //
      // The synch context already exists, probably created by the
      // receiver(s).
      //

      // Get the synch context.
      name [0].id =
        CORBA::string_dup ("Synch");

      CORBA::Object_var object =
        this->naming_->resolve (name
				ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      synch_context = CosNaming::NamingContext::_narrow (object.in ());

    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_CString synch_name ("Synch");
  ACE_Time_Value timestamp = ACE_OS::gettimeofday ();

  char buf [BUFSIZ];
  synch_name += CORBA::string_dup (ACE_OS::itoa (timestamp.sec (), buf, 10));

  name [0].id =
    CORBA::string_dup (synch_name.c_str ());

  ACE_DEBUG ((LM_DEBUG,
	      "Synch Name %s\n",
	      synch_name.c_str ()));
  
  ACE_NEW (synch_,
	   Synch_i);
  
  Synch_var synch = synch_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Register the synch object with the Synch context.
  synch_context->rebind (name,
			 synch.in ()
			 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}


int
DT_Creator::activate_root_poa (ACE_ENV_SINGLE_ARG_DECL)
{
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

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

void
DT_Creator::activate_poa_list (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"DT_Creator::activate_poa_list\n"));

  if (poa_count_ > 0)
    {
      CORBA::Object_var object =
	orb_->resolve_initial_references ("RTORB"
					  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->rt_orb_ =
	RTCORBA::RTORB::_narrow (object.in ()
				 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  
  for (int i = 0; i < poa_count_; ++i)
    {
      poa_list_[i]->activate (this->rt_orb_.in(), this->root_poa_.in ()
			      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
DT_Creator::activate_job_list (ACE_ENV_SINGLE_ARG_DECL)
{

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"DT_Creator::activate_job_list\n"));

  Job_i* job;

  for (int i = 0; i < job_count_; ++i)
    {
      job = job_list_[i];

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


    } /* while */

  ACE_DEBUG ((LM_DEBUG,
              "Activated Job List\n"));
}

void
DT_Creator::activate_schedule (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"Activating schedule, task count = %d\n",
		dt_count_));

  Thread_Task* task;

  for (int i = 0; i < dt_count_; ++i)
    {
      task = dt_list_[i];

      if (task->dist ())
	{
	  // resolve the object from the naming service
	  CosNaming::Name name (1);
	  name.length (1);
	  name[0].id = CORBA::string_dup (task->job ());

	  CORBA::Object_var obj =
	    this->naming_->resolve (name ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;

	  Job_var job = Job::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK;

	  //	  if (TAO_debug_level > 0)
	  // {
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
                  /*
		  RTCORBA::PriorityModel priority_model =
		    priority_policy->priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
		  ACE_CHECK;

		  if (priority_model == RTCORBA::CLIENT_PROPAGATED)
		    ACE_DEBUG ((LM_DEBUG,
				"%s priority_model = RTCORBA::CLIENT_PROPAGATED\n", task->job ()));
		  else
		    ACE_DEBUG ((LM_DEBUG,
				"%s priority_model = RTCORBA::SERVER_DECLARED\n", task->job ()));
		  */
		}
	      //} /*  if (TAO_debug_level > 0) */

	  task->job (job.in ());
	}
    }
  if (TAO_debug_level > 0 && dt_count_ > 0)
    ACE_DEBUG ((LM_DEBUG,
		"Activated schedule, task count = %d\n",
		dt_count_));

}

int
DT_Creator::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
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
DT_Creator::create_distributable_threads (RTScheduling::Current_ptr current
					  ACE_ENV_ARG_DECL)
{
  current_ = RTScheduling::Current::_duplicate (current);

  long flags;
  flags = THR_NEW_LWP | THR_JOINABLE;
  flags |=
    orb_->orb_core ()->orb_params ()->scope_policy () |
    orb_->orb_core ()->orb_params ()->sched_policy ();

  ACE_DEBUG ((LM_DEBUG,
              "Waiting to Synch\n"));
  
  while (!this->synch ()->synched ())
    {
      this->orb_->perform_work (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  
  CORBA::Policy_var sched_param;
  sched_param = CORBA::Policy::_duplicate (this->sched_param (100));
  const char * name = 0;
  current_->begin_scheduling_segment (name,
				      sched_param.in (),
				      sched_param.in ()
				      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW (base_time_,
	   ACE_Time_Value (*(this->synch ()->base_time ())));

  for (int i = 0; i < this->dt_count_; i++)
    {
      ACE_Time_Value now (ACE_OS::gettimeofday ());

      ACE_Time_Value elapsed_time =  now - *base_time_;

      char buf [BUFSIZ];
      ACE_OS::sprintf (buf, "elapsed time = %d\n now = %d\n base_time_ = %d\n",
		       (int) elapsed_time.sec (),
		       (int) now.sec (),
		       (int) base_time_->sec());

      log [log_index++] = ACE_OS::strdup (buf) ;

      ACE_hthread_t curr_thr;
      ACE_Thread::self (curr_thr);

      if (dt_list_ [i]->start_time () != 0 && (elapsed_time.sec () < dt_list_[i]->start_time ()))
	{
	  int suspension_time = dt_list_[i]->start_time () - elapsed_time.sec ();
	  ACE_OS::sprintf (buf,"suspension_tome = %d\n",
			   suspension_time);
	  log [log_index++] = ACE_OS::strdup (buf);
	  yield (suspension_time,
		 dt_list_[i]);
	}

      sched_param = CORBA::Policy::_duplicate (this->sched_param (dt_list_ [i]->importance ()));
      dt_list_ [i]->activate_task (current,
				   sched_param.in (),
				   flags,
				   base_time_
				   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

    }

  this->wait ();

  current_->end_scheduling_segment (name
				    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->check_ifexit ();
}

void
DT_Creator::dt_ended (void)
{
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_dt_count_;
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Active dt count = %d\n",active_dt_count_));
    char buf [BUFSIZ];
    ACE_OS::sprintf (buf,"Active dt count = %d\n",active_dt_count_);
    log [log_index++] = ACE_OS::strdup (buf);
  }
  this->check_ifexit ();
}

void
DT_Creator::job_ended (void)
{
  {
    ACE_GUARD (ACE_Lock, ace_mon, *state_lock_);
    --active_job_count_;
    char buf [BUFSIZ];
    ACE_OS::sprintf (buf,"Active job count = %d\n",active_job_count_);
    log [log_index++] = ACE_OS::strdup (buf);
  }

 this->check_ifexit ();
}

void
DT_Creator::check_ifexit (void)
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
	    for (int i = 0; i < dt_count_; i++)
	      {
		dt_list_[i]->dump_stats ();
	      }

	    for (int i = 0; i < job_count_; i ++)
	      {
		job_list_[i]->dump_stats ();
	      }
	    */
	    TASK_STATS::instance ()->dump_samples (file_name_,
						   "#Schedule Output",
						   ACE_High_Res_Timer::global_scale_factor ());

	    shutdown = 1;

	    FILE* log_file = ACE_OS::fopen (log_file_name_, "w");

	    if (log_file != NULL)
	      {
		// first dump what the caller has to say.
		ACE_OS::fprintf (log_file, "Log File\n");

		for (int i = 0; i < log_index; i++)
		  {
		    ACE_OS::fprintf (log_file, "%s\n", log [i]);
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
DT_Creator::dt_count (void)
{
  return dt_count_;
}

DT_Creator::~DT_Creator (void)
{
  // for (int i = 0; i < (BUFSIZ * 100); i++)
  delete[] log;

  delete[] dt_list_;
  delete[] poa_list_;
  delete[] job_list_;

  delete base_time_;

  delete state_lock_;
  delete shutdown_lock_;
}


void
DT_Creator::log_msg (char* msg)
{
  log [log_index++] = ACE_OS::strdup (msg);
}

CORBA::ORB_ptr
DT_Creator::orb (void)
{
  return this->orb_.in ();
}

void
DT_Creator::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
}

ACE_Time_Value*
DT_Creator::base_time (void)
{
  return this->base_time_;
}

void
DT_Creator::base_time (ACE_Time_Value* base_time)
{
  this->base_time_ = base_time;
}


RTScheduling::Current_ptr
DT_Creator::current (void)
{
  return current_.in ();
}


Synch_i*
DT_Creator::synch (void)
{
  return this->synch_;
}
