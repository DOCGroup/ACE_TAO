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
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"

#include "tao/corba.h"

#include "cubit_i.h"

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv(void);
extern "C" STATUS vmeDevCreate(char *);
#endif

// Global Variables
static CORBA::String key = CORBA::String ("Cubit");

const u_int OBJECT_STRING_SIZE = 30;
// size of the string for holding the stringified object reference

class Cubit_Task : public ACE_Task<ACE_NULL_SYNCH>
  // = TITLE
  //    Encapsulates an ORB for the Cubit application.
{
public:
  Cubit_Task (const char *args,
              const char* orbname,
              u_int num_of_objs);
  // Constructor.

  virtual int svc (void);
  // Active Object entry point.

protected:
  Cubit_Task (void) {};
  // No-op constructor.

private:
  char *orbname_;
  // Name of the ORB.

  char *orbargs_;
  // ORB arguments.

  u_int num_of_objs_;
  // Number of objects we're managing.

};

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
  CORBA::Environment env;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Beginning Cubit task with args = '%s'\n",
              orbargs_));
  ACE_ARGV args (orbargs_);

  int argc = args.argc ();
  char **argv = args.argv ();

  // Initialize the ORB.
  CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc, 
                                            argv, 
                                            this->orbname_, 
                                            env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB init");
      return 1;
    }

  // Initialize the Object Adapter.
  CORBA::POA_ptr oa_ptr = orb_ptr->POA_init (argc, argv, "POA");

  if (oa_ptr == 0)
    ACE_ERROR_RETURN ((LM_ERROR, " (%P|%t) Unable to initialize the POA.\n"), 1);

  // Create implementation object with user specified key.
  Cubit_i **my_cubit;

  ACE_NEW_RETURN (my_cubit, Cubit_i *[num_of_objs_], -1);

  u_int i;

  // this loop creates multiple servants
  for (i = 0; i < num_of_objs_; i++)
    {
      CORBA::String obj_str =
        CORBA::string_alloc (ACE_OS::strlen ((char *) key) 
                             + OBJECT_STRING_SIZE);

      ACE_OS::sprintf (obj_str, "%s%02d", (char *) key, i);

      ACE_NEW_RETURN (my_cubit[i], Cubit_i (obj_str), -1);

      if (my_cubit[i] == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to create "
			   "implementation object&d\n",
			   i), 2);

      CORBA::OctetSeq obj_key;

      obj_key.buffer = (CORBA::Octet *) obj_str;
      obj_key.length = obj_key.maximum = ACE_OS::strlen (obj_str);

      CORBA::Object_ptr obj = 0;

      if (oa_ptr->find (obj_key, obj) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to locate object with key '%s', %p\n",
                           key),
                          3);

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to some
      // client.  Then release the object.

      CORBA::String str;

      str = orb_ptr->object_to_string (obj, env);

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

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  orb_ptr->run ();

  // Shut down the OA -- recycles all underlying resources (e.g. file
  // descriptors, etc).

  //  oa_ptr->clean_shutdown (env);

  // Need to clean up and do a CORBA::release on everything we've
  // created!
  for (i = 0; i < num_of_objs_; i++)
    delete my_cubit[i];

  // Free resources
  CORBA::release (orb_ptr);
  CORBA::release (oa_ptr);

  return 0;
}

static char *hostname = NULL;
static int base_port = 0;
static int num_of_objs = 1;

// Parses the command line arguments and returns an error status

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
			   "\n", argv [0]), 1);
      }

  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
#if defined (VXWORKS) && defined (FORCE_ARGS)
main (int arg_c, char *arg_v[])
#else
main (int argc, char *argv[])
#endif
{
#if defined (VXWORKS)
   hostAdd("mv2604e", "130.38.183.178");
#if defined (VME_DRIVER)
   STATUS status = vmeDrv();
   if( status != OK )
      printf("ERROR on call to vmeDrv()\n");
   status = vmeDevCreate("/vme");
   if( status != OK )
      printf("ERROR on call to vmeDevCreate()\n");
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

  parse_args (argc, argv);

  if (hostname == 0 || base_port == 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "usage: %s "
                       " -p port -h my_hostname"
                       "\n",
                       argv[0]),
                      1);
  
  const size_t arg_size =
    ACE_OS::strlen ("rate20 -ORBport %d -ORBhost %s")
    + 5 /* port number */
    + MAXHOSTNAMELEN /* hostname */
    + 1 /* zero terminator */;
  
  char *args1 = 0;

  ACE_NEW_RETURN (args1, char[arg_size], -1);

  ::sprintf (args1,
             "rate20 -ORBport %d -ORBhost %s",
             base_port, 
             hostname);

  Cubit_Task high_priority_task (args1 , "internet", 1);

  ACE_Sched_Priority priority;

  // @@ Sumedh, can you please document why we need this VxWorks
  // #ifdef?  Is there a way to make this more general so we don't
  // need the #ifdef? 
  //   Doug, I didn't put this in. Maybe David knows why ?
#if defined (VXWORKS)
  priority = 
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, 
                                    ACE_SCOPE_THREAD) + 6;
#else
  priority = 
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, 
                                    ACE_SCOPE_THREAD);
#endif  /* VXWORKS */

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant with high priority\n"));

  // Make the high priority task an active object.
  high_priority_task.activate (THR_BOUND | ACE_SCHED_FIFO, 
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

  for (int i = 0; i < num_of_objs - 1; i++)
    {

      char *args = 0;
      ACE_NEW_RETURN (args, char[arg_size], -1);

      ::sprintf (args, 
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

  // Wait for the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();

  delete [] args1;
  
  return 0;
}
