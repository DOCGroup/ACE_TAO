// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    server.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include "server.h"
#include "ace/Sched_Params.h"

#if defined (NO_ACE_QUANTIFY)
#include "quantify.h"
#endif /* NO_ACE_QUANTIFY */

// Global options used to configure various parameters.
static char hostname[BUFSIZ];
static char *ior_file = 0;
static int base_port = ACE_DEFAULT_SERVER_PORT;
static u_int num_of_objs = 2;
static u_int use_name_service = 1;
static u_int thread_per_rate = 0;
static u_int use_multiple_priority = 0; 
static u_int run_utilization_test = 0;

Cubit_Task::Cubit_Task (void)
{
  // No-op.
}

Cubit_Task::Cubit_Task (const char *args,
                        const char *orbname,
                        u_int num_of_objs,
                        ACE_Barrier *barrier,
			Task_State *ts,
			ACE_Thread_Manager *thr_mgr,
                        u_int task_id)
  : ACE_MT (ACE_Task<ACE_MT_SYNCH> (thr_mgr)),
    key_ ("Cubit"),
    orbname_ ((char *) orbname),
    orbargs_ ((char *) args),
    num_of_objs_ (num_of_objs),
    servants_ (0),
    barrier_ (barrier),
    servants_iors_ (0),
    task_id_ (task_id),
    ts_ (ts)
{
}

int
Cubit_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Beginning Cubit task with args = '%s'\n",
              orbargs_));

  int rc = this->initialize_orb ();
  if (rc == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ORB initialization failed.\n"),
                      -1);

  rc = this->create_servants ();
  if (rc == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Create Servants failed.\n"),
                      -1);

  TAO_TRY
    {
      this->poa_manager_->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->barrier_->wait ();

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      if (this->orb_->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "run"),
                          -1);

      // Shut down the OA.
      this->poa_->destroy (CORBA::B_TRUE,
                           CORBA::B_TRUE,
                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("poa->destroy()");
    }
  TAO_ENDTRY;

  // Need to clean up and do a CORBA::release on everything we've
  // created!

  for (u_int i = 0; i < num_of_objs_; i++)
    delete servants_ [i];

  return 0;
}

int
Cubit_Task::initialize_orb (void)
{
  TAO_TRY
    {
      ACE_ARGV args (this->orbargs_);

      int argc = args.argc ();
      char **argv = args.argv ();

      // Initialize the ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    this->orbname_,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the Object Adapter.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ =
        PortableServer::POA::_narrow (poa_object.in(),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->poa_manager_ =
        this->root_poa_->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::PolicyList policies (2);
      policies.length (2);

      // Id Assignment policy
      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID,
                                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Lifespan policy
      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT,
                                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      this->poa_ =
        this->root_poa_->create_POA ("Persistent_POA",
                                     this->poa_manager_.in (),
                                     policies,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length () && TAO_TRY_ENV.exception () == 0;
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (TAO_TRY_ENV);
        }
      TAO_CHECK_ENV;

      if (use_name_service == 0)
        return 0;

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          1);

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);

      // Check the environment and return 1 if exception occurred or
      // nil pointer.
      if (TAO_TRY_ENV.exception () != 0 ||
          CORBA::is_nil (this->naming_context_.in ())==CORBA::B_TRUE )
        return 1;

      // Register the servant with the Naming Context....
      CosNaming::Name cubit_context_name (1);
      cubit_context_name.length (1);
      cubit_context_name[0].id =
        CORBA::string_dup ("MT_Cubit");

      TAO_TRY_ENV.clear ();
      CORBA::Object_var objref =
        this->naming_context_->bind_new_context (cubit_context_name,
                                                 TAO_TRY_ENV);

      if (TAO_TRY_ENV.exception() != 0)
        {
          CosNaming::NamingContext::AlreadyBound_ptr ex =
            CosNaming::NamingContext::AlreadyBound::_narrow (TAO_TRY_ENV.exception());
          if (ex != 0)
            {
              TAO_TRY_ENV.clear ();
              objref = this->naming_context_->resolve (cubit_context_name,
                                                       TAO_TRY_ENV);
              printf("NamingContext::AlreadyBound\n");
            }
          else
            TAO_TRY_ENV.print_exception ("bind() Cubit context object\n");
        }
      TAO_CHECK_ENV;

      this->mt_cubit_context_ =
        CosNaming::NamingContext::_narrow (objref.in (),
                                           TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("orb_init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

CORBA::String
Cubit_Task::get_servant_ior (u_int index)
{
  if (index >= num_of_objs_)
    return 0;

  return ACE_OS::strdup (this->servants_iors_[index]);
}

int
Cubit_Task::create_servants ()
{
  TAO_TRY
    {
      // Create the array of cubit implementations.
      ACE_NEW_RETURN (this->servants_,
                      Cubit_i *[this->num_of_objs_],
                      -1);

      // Create the array of strings.
      ACE_NEW_RETURN (this->servants_iors_,
                      CORBA::String [this->num_of_objs_],
                      -1);

      char *buffer;
      int l = ACE_OS::strlen (this->key_) + 3;

      ACE_NEW_RETURN (buffer,
                      char[l],
                      -1);

      // This loop creates multiple servants, and prints out their
      // IORs.
      for (u_int i = 0;
           i < this->num_of_objs_;
           i++)
        {
          ACE_OS::sprintf (buffer,
                           "%s%02d",
                           (char *) this->key_,
                           this->task_id_);

          PortableServer::ObjectId_var id =
            PortableServer::string_to_ObjectId (buffer);

          ACE_NEW_RETURN (this->servants_[i],
                          Cubit_i (ts_),
                          -1);

          if (this->servants_[i] == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Unable to create "
                               "implementation object #%d\n",
                               i),
                              2);

          this->poa_->activate_object_with_id (id.in (),
                                               this->servants_[i],
                                               TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Stringify the objref we'll be implementing, and print it
          // to stdout.  Someone will take that string and give it to
          // some client.  Then release the object.

          Cubit_var cubit =
            this->servants_[i]->_this (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CORBA::String_var str =
            this->orb_->object_to_string (cubit.in (),
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->servants_iors_[i] = ACE_OS::strdup (str.in ());

          // Register the servant with the Naming Context....
          CosNaming::Name cubit_name (1);
          cubit_name.length (1);
          cubit_name[0].id =
            CORBA::string_dup (buffer);

          if (CORBA::is_nil (this->mt_cubit_context_.in ()) == CORBA::B_FALSE)
            {
              this->mt_cubit_context_->bind (cubit_name,
                                             cubit.in (),
                                             TAO_TRY_ENV);
              if (TAO_TRY_ENV.exception () != 0)
                TAO_TRY_ENV.print_exception ("Attempt to bind() a cubit object to the name service Failed!\n");
              else
                ACE_DEBUG ((LM_DEBUG,
                            " (%t) Cubit object bound to the name \"%s\".\n",
                            buffer));
            }
        }
      delete [] buffer;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("print IOR");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// Parses the command line arguments and returns an error status.
// @@ This method should be integrated into one of the classes
// (preferably into an Options singleton) rather than kept as a
// stand-alone function.

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "sh:p:t:f:rmU");
  int c;

  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    {
      perror ("gethostname");
      return -1;
    }

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'U':
	run_utilization_test = 1;
	break;
      case 'm':
        use_multiple_priority = 1;
        break;
      case 'r':
        thread_per_rate = 1;
        break;
      case 's':
        use_name_service = 0;
        break;
      case 'f':
        ior_file = opts.optarg;
        break;
      case 'h':
        ACE_OS::strcpy (hostname, opts.optarg);
        ACE_DEBUG ((LM_DEBUG, "h\n"));
        break;
      case 'p':
        base_port = ACE_OS::atoi (opts.optarg);
        ACE_DEBUG ((LM_DEBUG, "p\n"));
        break;
      case 't':
        num_of_objs = ACE_OS::atoi (opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-s Means NOT to use the name service] "
                           " [-p <port>]"
                           " [-h <my_hostname>]"
                           " [-t <num_objects>]"
                           " [-f <ior_file>]"
                           " [-r Use thread per rate]"
                           "\n", argv [0]),
                          1);
      }

  if (thread_per_rate == 1)
    num_of_objs = 4;

  if (run_utilization_test == 1)
    num_of_objs = 1;

  // Indicates successful parsing of command line
  return 0;
}

// @@ This method should be integrated into one of the classes rather
// than kept as a stand-alone function.

static int
initialize (int argc, char **argv)
{
#if defined (VXWORKS)
#if defined (VME_DRIVER)
   STATUS status = vmeDrv ();

   if (status != OK)
     printf ("ERROR on call to vmeDrv()\n");

   status = vmeDevCreate ("/vme");

   if (status != OK)
     printf ("ERROR on call to vmeDevCreate()\n");
#endif /* defined (VME_DRIVER) */

#if defined (FORCE_ARGS)
   int argc = 4;
   char *argv[] = { "server",
                    "-s",
                    "-f",
                    "ior.txt" };

#endif /* defined (FORCE_ARGS) */
#endif /* defined (VXWORKS) */

   // Standard command line parsing utilities used.
   if (parse_args (argc, argv) != 0) return 1;

   if (hostname == 0 || base_port == 0)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "usage:  %s"
                        " [-s Means NOT to use the name service] "
                        " [-p port]"
                        " [-h my_hostname]"
                        " [-t num_objects]"
                        " [-f <ior_file>]"
                        " [-r Use thread per rate]"
                        "\n", argv [0]),
                       1);

   // Make sure we've got plenty of socket handles.  This call will use
   // the default maximum.
   ACE::set_handle_limit ();

   return 0;
}

// Starts up the servants

// @@ This method should be integrated into one of the classes rather
// than kept as a stand-alone function.

static int
start_servants (ACE_Thread_Manager *serv_thr_mgr, ACE_Barrier &start_barrier, Task_State *ts)
{
  char *args1;

  ACE_NEW_RETURN (args1,
                  char[BUFSIZ],
                  -1);
  u_int i;

  // Create an array to hold pointers to the Cubit objects.
  CORBA::String *cubits;

  ACE_NEW_RETURN (cubits,
                  CORBA::String [num_of_objs],
                  -1);

  ACE_OS::sprintf (args1,
                   "rate20 -ORBport %d "
                   "-ORBhost %s "
                   "-ORBobjrefstyle URL "
                   "-ORBsndsock 32768 "
                   "-ORBrcvsock 32768 ",
                   base_port,
                   hostname);

  Cubit_Task *high_priority_task;

  ACE_NEW_RETURN (high_priority_task,
                  Cubit_Task (args1,
                              "internet",
                              1,
                              &start_barrier,
			      ts,
			      serv_thr_mgr,
                              0), //task id 0.
                  -1);

#if defined (VXWORKS)
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF;
#elif defined (ACE_WIN32)
  ACE_Sched_Priority priority = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                                                ACE_SCOPE_THREAD);
#else
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF + 25;
#endif /* VXWORKS */

  if (run_utilization_test == 1)
     priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                 ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                                                 ACE_SCOPE_THREAD),
                                                 ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant 0 with high priority %d\n",
              priority));

  // Make the high priority task an active object.
  if (high_priority_task->activate (THR_BOUND | ACE_SCHED_FIFO,
                                    1,
                                    0,
                                    priority) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n",
                  "high_priority_task->activate"));
    }

  // Create an array to hold pointers to the low priority tasks.
  Cubit_Task **low_priority_task;

  ACE_NEW_RETURN (low_priority_task,
                  Cubit_Task *[num_of_objs],
                  -1);

  u_int number_of_low_priority_servants = 0;
  u_int number_of_priorities = 0;
  u_int grain = 0;
  u_int counter = 0;

  number_of_low_priority_servants = num_of_objs - 1;

  // Drop the priority
  if (thread_per_rate == 1 || use_multiple_priority == 1)
    {
      ACE_Sched_Priority_Iterator priority_iterator (ACE_SCHED_FIFO,
                                                     ACE_SCOPE_THREAD);

      number_of_priorities = 0;

      while (priority_iterator.more ())
        {
          number_of_priorities ++;
          priority_iterator.next ();
        }

      // 1 priority is exclusive for the high priority client.
      number_of_priorities --;

      // Drop the priority, so that the priority of clients will increase
      // with increasing client number.
      for (i = 0; i < number_of_low_priority_servants + 1; i++)
        priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                        priority,
                                                        ACE_SCOPE_THREAD);

      // granularity of the assignment of the priorities.  Some OSs have
      // fewer levels of priorities than we have threads in our test, so
      // with this mechanism we assign priorities to groups of threads when
      // there are more threads than priorities.
      grain = number_of_low_priority_servants / number_of_priorities;
      counter = 0;

      if (grain <= 0)
        grain = 1;

    }
  else
    {
      priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                      priority,
                                                      ACE_SCOPE_THREAD);
    }

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d servants starting at priority %d\n",
              number_of_low_priority_servants,
              priority));

  // Create the low priority servants.

  for (i = number_of_low_priority_servants; i > 0; i--)
    {
      char *args;

      ACE_NEW_RETURN (args,
                      char [BUFSIZ],
                      -1);

      ACE_OS::sprintf (args,
                       "rate10 -ORBport %d "
                       "-ORBhost %s "
                       "-ORBobjrefstyle URL "
                       "-ORBsndsock 32768 "
                       "-ORBrcvsock 32768 ",
                       base_port + i,
                       hostname);

      ACE_NEW_RETURN (low_priority_task [i - 1],
                      Cubit_Task (args, 
				  "internet", 
				  1, 
				  &start_barrier, 
				  ts,
				  serv_thr_mgr,
				  i),
                      -1);

      // Make the low priority task an active object.
      if (low_priority_task [i - 1]->activate (THR_BOUND | ACE_SCHED_FIFO,
                                           1,
                                           0,
                                           priority) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t; %p\n",
                      "low_priority_task[i]->activate"));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Created servant %d with priority %d\n",
                  i,
                  priority));

      // use different priorities on thread per rate or multiple priority.
      if (use_multiple_priority == 1 || thread_per_rate == 1)
        {
          counter = (counter + 1) % grain;
          if ( (counter == 0) &&
               //Just so when we distribute the priorities among the threads, we make sure we don't go overboard.
               ((number_of_priorities * grain) > (number_of_low_priority_servants - (i - 1))) )
              {
                      // Get the next higher priority.
                      priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                             priority,
                                                             ACE_SCOPE_THREAD);
              }

        }
    } /* end of for() */

  start_barrier.wait ();

  // Write the ior's to a file so the client can read them.
  {
    cubits[0] = high_priority_task->get_servant_ior (0);

    for (i = 0; i < num_of_objs-1; ++i)
      cubits[i + 1] = low_priority_task[i]->get_servant_ior (0);

    FILE *ior_f = 0;

    if (ior_file != 0)
      ior_f = ACE_OS::fopen (ior_file, "w+");

    for (i = 0; i < num_of_objs; ++i)
      {
        if (ior_f != 0)
          {
            ACE_OS::fprintf (ior_f, "%s\n", cubits[i]);
          }
        ACE_OS::printf ("cubits[%d] ior = %s\n",
                        i,
                        cubits[i]);
      }

    if (ior_f != 0)
      ACE_OS::fclose (ior_f);
  }
  return 0;

}

Util_Thread *
start_utilization (ACE_Thread_Manager *util_thr_mgr, Task_State *ts)
{
  Util_Thread *util_task;

  ACE_NEW_RETURN (util_task,
                  Util_Thread (ts,
			       util_thr_mgr),
		  0);

  ACE_Sched_Priority priority = ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
								ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG,
              "Creating Utilization Task with priority %d\n",
              priority));

  // Make the high priority task an active object.
  if (util_task->activate (THR_BOUND | ACE_SCHED_FIFO,
			   1,
			   0,
			   priority) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n",
                  "util_task->activate"));
    }

  return util_task;
}

// main routine.

#if defined (VXWORKS)
  // Rename main to server to make it easier to run both client and
  // server on one target.
extern "C"
int
server (int argc, char *argv[])
{
  ACE_Object_Manager ace_object_manager;
#else
int
main (int argc, char *argv[])
{
#endif

#if defined (ACE_HAS_THREADS)
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
#if defined (__Lynx__)
          ACE_THR_PRI_FIFO_DEF + 50,//30,
#elif defined (VXWORKS) /* ! __Lynx__ */
          6,
#elif defined (ACE_WIN32)
  ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                  ACE_SCOPE_THREAD),
#else
          ACE_THR_PRI_FIFO_DEF + 25,
#endif /* ! __Lynx__ */
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_DEBUG ((LM_MAX, "preempt: user is not superuser, "
                    "so remain in time-sharing class\n"));
      else
        ACE_ERROR_RETURN ((LM_ERROR, "%n: ACE_OS::sched_params failed\n%a"),
                          -1);
    }

  if (initialize (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Initialization\n"),
                      1);
  int _argc = 3;
  char *_argv[] = {"server",
                  "-t",
                  "1"};

  Task_State ts ( _argc, _argv);

  if (run_utilization_test == 1)
    ts.run_server_utilization_test_ = 1;

  Util_Thread * util_task = 0;

  // Create the daemon thread in its own <ACE_Thread_Manager>.
  ACE_Thread_Manager servant_thread_manager;
  ACE_Thread_Manager util_thr_mgr;
  ACE_Time_Value total_elapsed;
  double util_task_duration = 0.0;
  double total_latency = 0.0;
  double total_latency_servants = 0.0;
  double total_util_task_duration = 0.0;

  if (run_utilization_test == 1)
    {
      if ((util_task = start_utilization (&util_thr_mgr, &ts)) == 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "Error creating the utilization thread!\n"),
			  1);

      //
      // Time the utilization thread' "computation" to get %IdleCPU at the end of the test.
      //

#if defined (CHORUS)
      int pstartTime = 0;
      int pstopTime = 0;
      // Elapsed time will be in microseconds.
      pstartTime = pccTime1Get();
      // execute one computation.
      util_task->computation ();
      pstopTime = pccTime1Get();
      // Store the time in micro-seconds.
      util_task_duration = pstopTime - pstartTime;
#else /* CHORUS */
      ACE_High_Res_Timer timer_;
      // Elapsed time will be in microseconds.
      ACE_Time_Value delta_t;
      timer_.start ();
      // execute computation.
      for (int k=0; k < 1000; k++)
	util_task->computation ();
      timer_.stop ();
      timer_.elapsed_time (delta_t);
      // Store the time in milli-seconds.
      util_task_duration = (delta_t.sec () * 
			    ACE_ONE_SECOND_IN_USECS + 
			    (double)delta_t.usec ())  / 1000;
#endif /* !CHORUS */
    }
  // Barrier for the multiple clients to synchronize after binding to
  // the servants.
  ACE_Barrier start_barrier (num_of_objs + 1);

#if defined (NO_ACE_QUANTIFY)
  quantify_stop_recording_data();
  quantify_clear_data ();
  quantify_start_recording_data();
#endif /* NO_ACE_QUANTIFY */

  if (start_servants (&servant_thread_manager, start_barrier, &ts) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error creating the servants\n"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "Wait for all the threads to exit\n"));

  // Wait for all the threads to exit.
  servant_thread_manager.wait ();
  //  ACE_Thread_Manager::instance ()->wait ();

  if (run_utilization_test == 1)
    {
      util_task->done_ = 1;
    
      // This will wait for the utilization thread to finish.
      util_thr_mgr.wait ();
    
      ts.timer_.elapsed_time (total_elapsed);

      total_util_task_duration = util_task_duration * util_task->get_number_of_computations ();
    
      total_latency = (total_elapsed.sec () * 
		       ACE_ONE_SECOND_IN_USECS + 
		       (double)total_elapsed.usec ());
			      
      total_latency_servants = total_latency - total_util_task_duration;
    
      ACE_DEBUG ((LM_DEBUG,
		  "(%t) utilization task performed %g computations\n"
		  "(%t) each computation had a duration of %f msecs\n"
		  "(%t) elapsed time is %f msecs\n",
		  util_task->get_number_of_computations (),
		  util_task_duration / 1000,
		  total_latency / 1000));    

      // Calc and print the CPU percentage. I add 0.5 to round to the
      // nearest integer before casting it to int.
      ACE_DEBUG ((LM_DEBUG, 
		  "\t%% ORB Servant CPU utilization: %d %%\n"
		  "\t%% Idle time: %d %%\n",
		  (int) (total_latency_servants * 100 / total_latency + 0.5),
		  (int) (total_util_task_duration * 100 / total_latency + 0.5) ));    
    }

#if defined (NO_ACE_QUANTIFY)
  quantify_stop_recording_data();
#endif /* NO_ACE_QUANTIFY */

#else
  ACE_DEBUG ((LM_DEBUG,
              "Test not run.  This platform doesn't seem to have threads.\n"));
#endif /* ACE_HAS_THREADS */
  return 0;
}
