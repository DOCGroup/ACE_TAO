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
//    Andy Gokhale,
//    Sumedh Mungee,
//    Sergio Flores-Gaitan, and 
//    Nagarajan Surendran
//
// ============================================================================

#include "server.h"
#include "ace/Sched_Params.h"

#if defined (NO_ACE_QUANTIFY)
#include "quantify.h"
#endif /* NO_ACE_QUANTIFY */

ACE_RCSID(MT_Cubit, server, "$Id$")

Globals::Globals (void)
  : ior_file (0),
    base_port (0),
    num_of_objs (2),
    use_name_service (1),
    thread_per_rate (0),
    use_multiple_priority (0),
    run_utilization_test (0),
    ready_ (0),
    ready_cnd_ (ready_mtx_),
    barrier_ (0)
{
  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    ACE_OS::perror ("gethostname");
}

int
Globals::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "sh:p:t:f:rmU");
  int c;

  ACE_DEBUG ((LM_DEBUG,
              "%s",
              hostname));

  while ((c = opts ()) != -1)
    {
      //      ACE_DEBUG ((LM_DEBUG,"parse_args:%c ,",c));
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
        //        ACE_DEBUG ((LM_DEBUG,"Not using naming service\n"));
        use_name_service = 0;
        break;
      case 'f':
        //        ior_file = opts.optarg;
        ACE_NEW_RETURN (ior_file,
                        char[BUFSIZ],-1);
        ACE_OS::strcpy (ior_file,
                        opts.optarg);
        //        ACE_DEBUG ((LM_DEBUG,"Using file %s",ior_file));
        break;
      case 'h':
        ACE_OS::strcpy (hostname, opts.optarg);
        //        ACE_DEBUG ((LM_DEBUG, "h\n"));
        break;
      case 'p':
        base_port = ACE_OS::atoi (opts.optarg);
        ACE_DEBUG ((LM_DEBUG, "base_port:%d\n",base_port));
        break;
      case 't':
        num_of_objs = ACE_OS::atoi (opts.optarg);
        //        ACE_DEBUG ((LM_DEBUG,"num_of_objs:%d\n",num_of_objs));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " \t[-s Means NOT to use the name service] \n"
                           " \t[-p <port>]\n"
                           " \t[-h <my_hostname>]\n"
                           " \t[-t <num_objects>]\n"
                           " \t[-f <ior_file>]\n"
                           " \t[-r Use thread per rate]\n"
                           "\n", argv [0]),
                          -1);
      }
    }
  if (thread_per_rate == 1)
    num_of_objs = 4;

  if (run_utilization_test == 1)
    num_of_objs = 1;

  // Indicates successful parsing of command line
  return 0;
}

Cubit_Task::Cubit_Task (void)
{
  // No-op.
}

Cubit_Task::Cubit_Task (const char *args,
                        const char *orbname,
                        u_int num_of_objs,
                        Task_State *ts,
                        ACE_Thread_Manager *thr_mgr,
                        u_int task_id)
  : ACE_MT (ACE_Task<ACE_MT_SYNCH> (thr_mgr)),
    key_ ("Cubit"),
    orbname_ ((char *) orbname),
    orbargs_ ((char *) args),
    num_of_objs_ (num_of_objs),
    servants_ (0),
    servants_iors_ (0),
    task_id_ (task_id),
    ts_ (ts)
{
}

int
Cubit_Task::svc (void)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  // thr_getprio () on the current thread should never fail.
  ACE_OS::thr_getprio (thr_handle, prio);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Beginning Cubit task with args = '%s' and priority %d\n",
              orbargs_,
              prio));

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

      GLOBALS::instance ()->barrier_->wait ();

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
    delete this->servants_[i];

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

      // @@ Naga, can you please try to use the TAO_Object_Manager for
      // all of this initialization, rather than doing it all by hand?

      // Initialize the ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    this->orbname_,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the Object Adapter.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ =
        PortableServer::POA::_narrow (poa_object.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->poa_manager_ =
        this->root_poa_->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->poa_manager_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Root poa manager is nil\n"),
                          -1);
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

      // Do the argument parsing.

      if (this->task_id_ == 0)
        {
          //          ACE_DEBUG ((LM_DEBUG,"parsing the arguments\n"));
          if (GLOBALS::instance ()->parse_args (argc,
                                                argv) < 0)
            return -1;
          ACE_NEW_RETURN (GLOBALS::instance ()->barrier_,
                          ACE_Barrier (GLOBALS::instance ()->num_of_objs + 1),
                          -1);
          //          ACE_DEBUG ((LM_DEBUG,"(%t)Arguments parsed successfully\n"));
          ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ready_mon, GLOBALS::instance ()->ready_mtx_, 1));
          GLOBALS::instance ()->ready_ = 1;
          GLOBALS::instance ()->ready_cnd_.broadcast ();
          ready_mon.release ();
          if (GLOBALS::instance ()->barrier_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%t)Unable to create barrier\n"),
                              -1);
        }

      if (GLOBALS::instance ()->use_name_service == 0)
        return 0;

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);

      // Check the environment and return 1 if exception occurred or
      // nil pointer.
      if (TAO_TRY_ENV.exception () != 0 ||
          CORBA::is_nil (this->naming_context_.in ())==CORBA::B_TRUE )
        return -1;

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
Cubit_Task::create_servants (void)
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
      // @@ Naga, can you please document why the value "3" is here?
      int len = ACE_OS::strlen (this->key_) + 3;

      ACE_NEW_RETURN (buffer,
                      char[len],
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



int
Server::initialize (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;
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
   char *argv[] = 
   {
     "server",
     "-s",
     "-f",
     "ior.txt" 
   };
#endif /* defined (FORCE_ARGS) */
#endif /* defined (VXWORKS) */

   // Make sure we've got plenty of socket handles.  This call will
   // use the default maximum.
   ACE::set_handle_limit ();
   return 0;
}

int
Server::start_servants (ACE_Thread_Manager *serv_thr_mgr,
                        Task_State *ts)
{

  ACE_ARGV tmp_args (this->argv_);
  const char *arg_buf = tmp_args.buf ();

  char *low_thread_args;

  int arg_len = ACE_OS::strlen (arg_buf);

  ACE_NEW_RETURN (low_thread_args,
                  char[arg_len + 1],
                  -1);

  ACE_OS::strcpy (low_thread_args,
                  arg_buf);
  char *args1;

  // @@ Naga, can you please explain why you need to do all of this?
  // i.e, we need some comments here!  In particular, what is args1
  // being used for and how will we know that ACE_DEFAULT_ARGV_BUFSIZ
  // is an appropriate size?  It seems to me that we should either (1)
  // add an accessor on ACE_ARGV to determine what this size ought to
  // be or (2) we should try to use/add a method on ACE_ARGV that
  // converts the argv back into a char * buffer or something!  At any
  // rate, this code should be cleaned up and abstracted better.
  ACE_NEW_RETURN (args1,
                  char[ACE_DEFAULT_ARGV_BUFSIZ],
                  -1);
  int i;

  for (i = 0; i < this->argc_ ; i++)
    {
      if (ACE_OS::strcmp (this->argv_[i], "-p") == 0
          && i - 1 < this->argc_)
        {
          GLOBALS::instance ()->base_port =
            ACE_OS::atoi (this->argv_[i + 1]);
          ACE_DEBUG ((LM_DEBUG,
                      "base_port:%d",
                      GLOBALS::instance()->base_port));
        }
      else if (ACE_OS::strcmp (this->argv_[i], "-h") == 0
               && i - 1 < this->argc_)
        ACE_OS::strcpy (GLOBALS::instance ()->hostname,
                        this->argv_[i+1]);
      else if (ACE_OS::strcmp (this->argv_[i], "-t") == 0
               && i - 1 < this->argc_)
        GLOBALS::instance ()->num_of_objs =
          ACE_OS::atoi (this->argv_ [i + 1]);
    }
  // Create an array to hold pointers to the Cubit objects.
  CORBA::String *cubits;

  ACE_NEW_RETURN (cubits,
                  CORBA::String [GLOBALS::instance ()->num_of_objs],
                  -1);
  ACE_OS::sprintf (args1,
                   "-ORBport %d "
                   "-ORBhost %s "
                   "-ORBobjrefstyle URL "
                   "-ORBsndsock 32768 "
                   "-ORBrcvsock 32768 ",
                   GLOBALS::instance ()->base_port,
                   GLOBALS::instance ()->hostname);

  char *high_thread_args;
  int args1_len = ACE_OS::strlen (args1);

  ACE_NEW_RETURN (high_thread_args,
                  char [arg_len + args1_len +1],
                  -1);

  ACE_OS::strcpy (high_thread_args, arg_buf);
  ACE_OS::strcat (high_thread_args, args1);
  Cubit_Task *high_priority_task;

  ACE_NEW_RETURN (high_priority_task,
                  Cubit_Task (high_thread_args,
                              "internet",
                              1,
                              ts,
                              serv_thr_mgr,
                              0), //task id 0.
                  -1);

#if defined (VXWORKS)
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF;
#elif defined (ACE_WIN32)
  ACE_Sched_Priority priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                    ACE_SCOPE_THREAD);
#else
  // @@ Naga/Sergio, why is there a "25" here?  This seems like to
  // much of a "magic" number.  Can you make this more "abstract?"
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF + 25;
#endif /* VXWORKS */

  if (GLOBALS::instance ()->run_utilization_test == 1)
    priority =
      ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
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
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n"
                "\thigh_priority_task->activate failed"));

  //  ACE_DEBUG ((LM_DEBUG,"(%t) Waiting for argument parsing\n"));
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ready_mon, GLOBALS::instance ()->ready_mtx_,-1));

  while (!GLOBALS::instance ()->ready_)
    GLOBALS::instance ()->ready_cnd_.wait ();

  //  ACE_DEBUG ((LM_DEBUG,"(%t) Argument parsing waiting done\n"));

  if (GLOBALS::instance ()->run_utilization_test == 1)
    priority = 
      ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                       ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                                       ACE_SCOPE_THREAD),
                                       ACE_SCOPE_THREAD);

  // Create an array to hold pointers to the low priority tasks.
  Cubit_Task **low_priority_task;

  ACE_NEW_RETURN (low_priority_task,
                  Cubit_Task *[GLOBALS::instance ()->num_of_objs],
                  -1);

  u_int number_of_low_priority_servants = 0;
  u_int number_of_priorities = 0;
  u_int grain = 0;
  u_int counter = 0;
  u_int j;

  number_of_low_priority_servants =
    GLOBALS::instance ()->num_of_objs - 1;

  // Drop the priority
  if (GLOBALS::instance ()->thread_per_rate == 1 
      || GLOBALS::instance ()->use_multiple_priority == 1)
    {
      number_of_priorities = 0;

      for (ACE_Sched_Priority_Iterator priority_iterator (ACE_SCHED_FIFO,
                                                          ACE_SCOPE_THREAD);
           priority_iterator.more ();
          priority_iterator.next ())
        number_of_priorities ++;

      // 1 priority is exclusive for the high priority client.
      number_of_priorities --;

      // Drop the priority, so that the priority of clients will
      // increase with increasing client number.
      for (j = 0;
           j < number_of_low_priority_servants;
           j++)
        priority =
          ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                               priority,
                                               ACE_SCOPE_THREAD);

      // Granularity of the assignment of the priorities.  Some OSs
      // have fewer levels of priorities than we have threads in our
      // test, so with this mechanism we assign priorities to groups
      // of threads when there are more threads than priorities.
      grain = number_of_low_priority_servants / number_of_priorities;
      counter = 0;

      if (grain <= 0)
        grain = 1;
    }
  else
    priority =
      ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                           priority,
                                           ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d servants starting at priority %d\n",
              number_of_low_priority_servants,
              priority));

  // Create the low priority servants.

  for (i = number_of_low_priority_servants; i > 0; i--)
    {
      char *args;

      ACE_NEW_RETURN (args,
                      char [ACE_DEFAULT_ARGV_BUFSIZ],
                      -1);
      ACE_OS::sprintf (args,
                       "-ORBport %d "
                       "-ORBhost %s "
                       "-ORBobjrefstyle URL "
                       "-ORBsndsock 32768 "
                       "-ORBrcvsock 32768 ",
                       (GLOBALS::instance ()->base_port == 0) ? (int) 0 :GLOBALS::instance ()->base_port+i,
                       GLOBALS::instance ()->hostname);

      int args_len = ACE_OS::strlen (args);
      char *new_args;
      
      ACE_NEW_RETURN  (new_args,
                       char [arg_len + args_len +1],
                       -1);

      ACE_OS::strcat (new_args,args);

      ACE_NEW_RETURN (low_priority_task [i - 1],
                      Cubit_Task (new_args,
				  "internet",
				  1,
				  ts,
				  serv_thr_mgr,
				  i),
                      -1);

      // Make the low priority task an active object.
      if (low_priority_task [i - 1]->activate (THR_BOUND | ACE_SCHED_FIFO,
                                           1,
                                           0,
                                           priority) == -1)
        ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n"
                    "\tlow_priority_task[i]->activate"));
      ACE_DEBUG ((LM_DEBUG,
                  "Created servant %d with priority %d\n",
                  i,
                  priority));

      // Use different priorities on thread per rate or multiple
      // priority.
      if (GLOBALS::instance ()->use_multiple_priority == 1 
          || GLOBALS::instance ()->thread_per_rate == 1)
        {
          counter = (counter + 1) % grain;
          if (counter == 0
              &&
               //Just so when we distribute the priorities among the
               //threads, we make sure we don't go overboard.
              (number_of_priorities * grain > number_of_low_priority_servants - (i - 1)))
            // Get the next higher priority.
            priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                        priority,
                                                        ACE_SCOPE_THREAD);

        }
    } /* end of for() */

  GLOBALS::instance ()->barrier_->wait ();

  // Write the ior's to a file so the client can read them.
  {
    cubits[0] = high_priority_task->get_servant_ior (0);

    for (j = 0;
         j < GLOBALS::instance ()->num_of_objs-1;
         ++j)
      cubits[j + 1] = low_priority_task[j]->get_servant_ior (0);

    FILE *ior_f = 0;

    if (GLOBALS::instance ()->ior_file != 0)
      {
        //        ACE_DEBUG ((LM_DEBUG,"(%P|%t) Opening file:%s\n",GLOBALS::instance ()->ior_file));
        ior_f = ACE_OS::fopen (GLOBALS::instance ()->ior_file, "w");
      }

    for (j = 0;
         j < GLOBALS::instance ()->num_of_objs;
         ++j)
      {
        if (ior_f != 0)
          {
            //            ACE_DEBUG ((LM_DEBUG,"(%P|%t) ior_file is open :%s",GLOBALS::instance ()->ior_file));
            ACE_OS::fprintf (ior_f, "%s\n", cubits[j]);
            ACE_OS::printf ("cubits[%d] ior = %s\n",
                            j,
                            cubits[j]);
          }
      }

    if (ior_f != 0)
      {
        //        ACE_DEBUG ((LM_DEBUG,"(%P|%t) Closing ior file\n"));
        ACE_OS::fclose (ior_f);
      }
  }
  return 0;

}

Util_Thread *
Server::start_utilization (ACE_Thread_Manager *util_thr_mgr,
                           Task_State *ts)
{
  Util_Thread *util_task;

  ACE_NEW_RETURN (util_task,
                  Util_Thread (ts,
                               util_thr_mgr),
                  0);

  ACE_Sched_Priority priority =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
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
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n"
                  "\tutil_task->activate failed"));
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
  // Dummy code to create the GLOBALS object in the global memory
  // instead of TSS.
  GLOBALS::instance ();

  Server server;
#if defined (ACE_HAS_THREADS)
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
#if defined (__Lynx__)
          30,
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

  if (server.initialize (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Initialization\n"),
                      1);
  int _argc = 3;
  char *_argv[] = {"server",
                  "-t",
                  "1"};

  Task_State ts ( _argc, _argv);

  // parse the arguments to set the global task state.
  ts.parse_args (_argc,_argv);

  // preliminary argument processing
  
  for (int i=0;i<argc;i++)
    if (ACE_OS::strcmp (argv[i],"-U")==0)
      GLOBALS::instance ()->run_utilization_test =1;

  if (GLOBALS::instance ()->run_utilization_test == 1)
    {
      ts.run_server_utilization_test_ = 1;
      ts.loop_count_ = 0;
     }

  Util_Thread *util_task = 0;

  // Create the daemon thread in its own <ACE_Thread_Manager>.
  ACE_Thread_Manager servant_thread_manager;
  ACE_Thread_Manager util_thr_mgr;
  ACE_Time_Value total_elapsed;
  double util_task_duration = 0.0;
  double total_latency = 0.0;
  double total_latency_servants = 0.0;
  double total_util_task_duration = 0.0;

  if (GLOBALS::instance ()->run_utilization_test == 1)
    {
      if ((util_task = server.start_utilization (&util_thr_mgr, &ts)) == 0)
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
      for (int k=0; k < 10000; k++)
        util_task->computation ();
      timer_.stop ();
      timer_.elapsed_time (delta_t);
      // Store the time in milli-seconds.
      util_task_duration = ((double)delta_t.sec () *
                            ACE_ONE_SECOND_IN_USECS +
                            (double)delta_t.usec ())  / 10000;
#endif /* !CHORUS */
    }

#if defined (NO_ACE_QUANTIFY)
  quantify_stop_recording_data();
  quantify_clear_data ();
  quantify_start_recording_data();
#endif /* NO_ACE_QUANTIFY */

  if (server.start_servants (&servant_thread_manager,&ts) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error creating the servants\n"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "Wait for all the threads to exit\n"));

  // Wait for all the threads to exit.
  servant_thread_manager.wait ();
  //  ACE_Thread_Manager::instance ()->wait ();

  if (GLOBALS::instance ()->run_utilization_test == 1)
    {
      util_task->done_ = 1;

      // This will wait for the utilization thread to finish.
      util_thr_mgr.wait ();

      ts.timer_.elapsed_time (total_elapsed);

      total_util_task_duration =
        util_task_duration * util_task->get_number_of_computations ();

      total_latency = (total_elapsed.sec () *
                       ACE_ONE_SECOND_IN_USECS +
                       (double)total_elapsed.usec ());

      total_latency_servants = total_latency - total_util_task_duration;

      ACE_DEBUG ((LM_DEBUG,
                  "-------------------------- Stats -------------------------------\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) UTILIZATION task performed \t%u computations\n"
                  "(%t) SERVANT task serviced \t\t%u CORBA calls\n"
                  "\t Ratio of computations to CORBA calls is %u.%u:1\n\n",
                  util_task->get_number_of_computations (),
                  ts.loop_count_,
                  util_task->get_number_of_computations () / ts.loop_count_,
                  (util_task->get_number_of_computations () % ts.loop_count_) * 100 / ts.loop_count_
                  ));

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Each computation had a duration of %f msecs\n"
                  "(%t) Total elapsed time of test is %f msecs\n",
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Globals,ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Globals,ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
