// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    svr.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include "server.h"

Cubit_Task::Cubit_Task (void)
{
  // No-op.
}

Cubit_Task::Cubit_Task (const char *args,
                        const char *orbname,
                        u_int num_of_objs)
  : orbname_ ((char *) orbname),
    orbargs_ ((char *) args),
    num_of_objs_ (num_of_objs)
{
}

int
Cubit_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Beginning Cubit task with args = '%s'\n",
              orbargs_));
  this->initialize_orb ();
  this->create_servants ();

  TAO_TRY
    {
      this->poa_manager_->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Handle requests for this object until we're killed, or one of the
      // methods asks us to exit.
      if (this->orb_->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);
	

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

      // Initialize the Object Adapter
      CORBA::Object_var poa_object = 
	this->orb_->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      this->root_poa_ =
	PortableServer::POA::_narrow (poa_object.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->poa_manager_ =
	this->root_poa_->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::PolicyList policies (2);
      policies.length (2);  
      policies[0] =
	this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID,
						      TAO_TRY_ENV);
      policies[1] =
	this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT,
						 TAO_TRY_ENV);

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      this->poa_ =
	this->root_poa_->create_POA ("Persistent_POA",
				     this->poa_manager_.in (),
				     policies,
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

int
Cubit_Task::create_servants ()
{
  TAO_TRY
    {
      // Create implementation object with user specified key.

      ACE_NEW_RETURN (servants_,
		      Cubit_i *[num_of_objs_],
		      -1);

      char* buffer;
      int l = ACE_OS::strlen (key) + 3;
      ACE_NEW_RETURN (buffer, char[l], -1);

      // This loop creates multiple servants, and prints out their IORs
      for (u_int i = 0;
	   i < num_of_objs_;
	   i++)
	{
	  ACE_OS::sprintf (buffer, "%s%02d",
			   (char *) key, i);

	  PortableServer::ObjectId_var id = 
	    PortableServer::string_to_ObjectId (buffer);

	  ACE_NEW_RETURN (this->servants_[i], Cubit_i, -1);

	  if (this->servants_[i] == 0)
	    ACE_ERROR_RETURN ((LM_ERROR,
			       " (%P|%t) Unable to create "
			       "implementation object&d\n",
			       i), 2);

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
	    this->orb_->object_to_string (cubit.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  
	  ACE_DEBUG ((LM_DEBUG,
		      "Object <%s> created\n",
		      str.in ()));
	}
      delete[] buffer;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("print IOR");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// Global options used to configure various parameters.
static char *hostname = NULL;
static int base_port = 0;
static int num_of_objs = 1;

// Parses the command line arguments and returns an error status.

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "h:p:t:");
  int c;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'h':
        hostname = opts.optarg;
        break;
      case 'p':
	base_port = ACE_OS::atoi (opts.optarg);
	break;
      case 't':
	num_of_objs = ACE_OS::atoi (opts.optarg);
	break;
      case '?':
      default:
	ACE_ERROR_RETURN ((LM_ERROR,
			   "usage:  %s"
			   " -p port"
			   " -h my_hostname"
                           " -t num_objects"
			   "\n", argv [0]),
                          1);
      }

  // Indicates successful parsing of command line
  return 0;  
}

int
initialize (int argc, char **argv)
{
#if defined (VXWORKS)
   hostAdd ("mv2604e", "130.38.183.178");
#if defined (VME_DRIVER)
   STATUS status = vmeDrv ();
   if (status != OK)
     printf ("ERROR on call to vmeDrv()\n");
   status = vmeDevCreate ("/vme");
   if (status != OK)
     printf ("ERROR on call to vmeDevCreate()\n");
#endif /* defined (VME_DRIVER) */

#if defined (FORCE_ARGS)
   int argc = 5;
   char *argv[] = { "main",
                    "-p",
                    "10014",
                    "-h",
                    "130.38.183.132" };
   
#endif /* defined (FORCE_ARGS) */
#endif /* defined (VXWORKS) */

   // Standard command line parsing utilities used.
  parse_args (argc, argv);

  if (hostname == 0 || base_port == 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "usage:  %s"
			   " -p port"
			   " -h my_hostname"
                           " -t num_objects"
			   "\n", argv [0]),
                          1);
  return 0;
}

// Starts up the servants
int
start_servants ()
{
  char *args1 = new char [4096];
  ACE_OS::sprintf (args1,
                   "rate20 -ORBport %d -ORBhost %s",
                   base_port, 
                   hostname);

  Cubit_Task *high_priority_task;
  ACE_NEW_RETURN (high_priority_task, 
                  Cubit_Task (args1,
                              "internet",
                              1),
                  1);
                    
  ACE_Sched_Priority priority;

  // @@ The ifdef here is temporarily placed here until
  // I figure out how to map NT's thread priorities
  // into pthread's priorities.
#if defined (ACE_THR_PRI_FIFO_DEF)
  priority = ACE_THR_PRI_FIFO_DEF;
#else
  priority = ACE_DEFAULT_THREAD_PRIORITY;
#endif /* ACE_THR_PRI_FIFO_DEF */

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant with high priority\n"));

  // Make the high priority task an active object.
  high_priority_task->activate (THR_BOUND | ACE_SCHED_FIFO, 
                               1, 
                               0, 
                               priority);

  // Create an array to hold pointers to the low priority tasks..
  Cubit_Task **low_priority_task;

  ACE_NEW_RETURN (low_priority_task, 
                  Cubit_Task *[num_of_objs], 
                  -1);

  priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG, 
              "Creating %d servants with low priority\n",
              num_of_objs - 1));

  // Create the low priority servants.
  for (int i = 0; i < num_of_objs - 1; i++)
    {

      char *args = new char [4096];

      ACE_OS::sprintf (args, 
                       "rate10 -ORBport %d -ORBhost %s", 
                       base_port + 1 + i,
                       hostname);

      ACE_NEW_RETURN (low_priority_task [i],
                      Cubit_Task (args, "internet", 1),
                      -1);

      priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_THREAD);

      // Make the low priority task an active object.
      low_priority_task [i]->activate (THR_BOUND | ACE_SCHED_FIFO, 
                                       1, 
                                       0, 
                                       priority);

    }
  return 0;
}

// main routine.

int
main (int argc, char *argv[])
{
  if (initialize (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Error in Initialization\n"), 1);
  if(start_servants () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Error creating the servants\n"), 1);
  // Wait for all the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

