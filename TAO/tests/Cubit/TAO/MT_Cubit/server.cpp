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
  : orb_ptr_ (0),
    oa_ptr_ (0)
{
  // No-op.
}

Cubit_Task::Cubit_Task (const char *args,
                        const char *orbname,
                        u_int num_of_objs)
  : orbname_ ((char *) orbname),
    orbargs_ ((char *) args),
    num_of_objs_ (num_of_objs),
    orb_ptr_ (0),
    oa_ptr_ (0)
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

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  orb_ptr_->run ();

  // Shut down the OA -- recycles all underlying resources (e.g. file
  // descriptors, etc).

  //  oa_ptr_->clean_shutdown (env);

  // Need to clean up and do a CORBA::release on everything we've
  // created!
  for (u_int i = 0; i < num_of_objs_; i++)
    delete servants_ [i];

  // Free resources
  CORBA::release (orb_ptr_);
  CORBA::release (oa_ptr_);

  return 0;
}

int
Cubit_Task::initialize_orb (void)
{
  CORBA::Environment env;
 
  ACE_ARGV args (this->orbargs_);

  int argc = args.argc ();
  char **argv = args.argv ();

  // Initialize the ORB.
  orb_ptr_ = CORBA::ORB_init (argc, 
                              argv, 
                              this->orbname_, 
                              env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB init");
      return 1;
    }

  // Initialize the Object Adapter.
  oa_ptr_ = orb_ptr_->POA_init (argc, argv, "POA");

  if (oa_ptr_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize the POA.\n"),
                      1);
  return 0;
}

int
Cubit_Task::create_servants ()
{
  // Create implementation object with user specified key.

  ACE_NEW_RETURN (servants_,
                  Cubit_i *[num_of_objs_],
                  -1);

  u_int i;

  // This loop creates multiple servants, and prints out their IORs
  for (i = 0;
       i < num_of_objs_;
       i++)
    {
      CORBA::String obj_str =
        CORBA::string_alloc (ACE_OS::strlen ((char *) key) 
                             + OBJECT_STRING_SIZE);

      ACE_OS::sprintf (obj_str,
                       "%s%02d",
                       (char *) key,
                       i);

      ACE_NEW_RETURN (servants_[i],
                      Cubit_i (obj_str),
                      -1);

      if (servants_[i] == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to create "
			   "implementation object&d\n",
			   i), 2);

      CORBA::OctetSeq obj_key;

      obj_key.buffer = (CORBA::Octet *) obj_str;
      obj_key.length = obj_key.maximum = ACE_OS::strlen (obj_str);

      CORBA::Object_ptr obj = 0;

      if (oa_ptr_->find (obj_key, obj) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to locate object with key '%s', %p\n",
                           key),
                          3);

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to some
      // client.  Then release the object.

      CORBA::String str;

      CORBA::Environment env;

      str = orb_ptr_->object_to_string (obj, env);

      if (env.exception () != 0)
        {
          env.print_exception ("object2string");
          return 1;
        }

      ACE_OS::puts ((char *) str);
      ACE_OS::fflush (stdout);
      ACE_DEBUG ((LM_DEBUG,"Object Created at: '%ul'", obj));
      CORBA::string_free (obj_str);
    }

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

  priority = ACE_THR_PRI_FIFO_DEF;

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

