// $Id$

// @(#)svr.cpp	05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	simple IIOP server for "cubit.idl" interface.
//
// Starts up, builds an objref, prints its string in debug mode,
// listens for messages, responds to them.
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

#include "pbroker/corba/orb.h"
#include "pbroker/corba/request.h"
#include "pbroker/corba/environ.h"
#include "pbroker/unixsvc/unixsvc.h"
#include "pbroker/corba/xpsorb.h"
#include "pbroker/corba/xpsboa.h"
#include "pbroker/corba/boad/boafull.h"   // will force usage of "Full" BOA API
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/Task.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"

#include "cubit_i.h"

ACE_RCSID(server, svr, "$Id$")

// Global Variables
static CORBA::String key = (CORBA::String) "Cubit";


class Cubit_Task : public ACE_Task<ACE_NULL_SYNCH>
// = TITLE
//    Encapsulates an ORB for the Cubit application
{
public:
  Cubit_Task (const char* args, const char* orbname, unsigned int num_of_objs); //

  virtual int svc (void);

protected:
  Cubit_Task () {};

private:
  char* orbname_;
  char* orbargs_;
  unsigned int num_of_objs_;
};

Cubit_Task::Cubit_Task (const char* args, const char* orbname, unsigned int num_of_objs)
  : orbname_ ((char*)orbname),
    orbargs_ ((char*)args),
    num_of_objs_ (num_of_objs)
{}


int
Cubit_Task::svc (void)
{
  CORBA::Environment env;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Beginning Cubit task with args = '%s'\n", orbargs_));
  ACE_ARGV args (orbargs_);

  int argc = args.argc ();
  char** argv = args.argv ();

  XpsORB_ptr       orb_ptr = XpsORB::_nil();
  XpsBOA *         oa_ptr  = XpsBOA::_nil ();
  CORBA_Object_ptr obj_ptr = CORBA_Object::_nil();

  try {
    // Initialize the ORB pointer
    orb_ptr = XpsORB::init (argc, argv); 
    
    // Initialize the Object Adapter
    oa_ptr = XpsBOA::init (argc, argv);
    
    if (CORBA::is_nil(orb_ptr) || CORBA::is_nil(oa_ptr))
      {
	ACE_ERROR_RETURN ((LM_ERROR, 
			   " (%P|%t) Unable to initialize the ORB and/or the BOA\n"),
			  1);
      }
    
  // Create implementation object with user specified key.
  Cubit_i_ptr *my_cubit = new Cubit_i_ptr[num_of_objs_];

  u_int i;
  for (i = 0; i < num_of_objs_; i++)
    {
      CORBA::String obj_str = CORBA::string_alloc (ACE_OS::strlen ((char *) key)+30);

#if 0
      ACE_OS::sprintf (obj_str, "%s-%u-%02d", (char *) key, ACE_Thread::self (), i);
#else
      ACE_OS::sprintf (obj_str, "%s%02d", (char *) key, i);
#endif

      my_cubit[i] = new Cubit_i (obj_str);

      if (my_cubit[i] == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to create implementation object&d\n",
			   i),
			  2);

      //CORBA::OctetSeq obj_key;
      //obj_key.buffer = (CORBA::Octet *) obj_str;
      //obj_key.length = obj_key.maximum = ACE_OS::strlen (obj_str);

      //CORBA::Object_ptr obj = 0;

      //      if (oa_ptr->find (obj_key, obj) == -1)
      //   ACE_ERROR_RETURN ((LM_ERROR,
      //                     " (%P|%t) Unable to locate object with key '%s', %p\n",
      //                     key),
      //                    3);

      // Stringify the objref we'll be implementing, and print it
      // to stdout.  Someone will take that string and give it to
      // some client.  Then release the object.

      CORBA::String str;

      //  str = orb_ptr->object_to_string (obj);
      str = orb_ptr->object_to_url (my_cubit[i]);
      
      ACE_DEBUG ((LM_DEBUG, "listening as object '%s'\n", str));
      CORBA_string_free (str);

      oa_ptr->obj_is_ready (my_cubit[i], nil);
      oa_ptr->registerAlias (my_cubit[i], obj_str);

      if (env.exception () != 0)
        {
	  //          env.print_exception ("object2string");
          return 1;
        }

      ACE_OS::puts ((char *) str);
      ACE_OS::fflush (stdout);
      //ACE_DEBUG ((LM_DEBUG,"Object Created at: '%ul'", obj));
      CORBA::string_free (obj_str);
    }

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  XpsEventService  eventService;
  eventService.mainloop();   // control will not return to this point
  //  orb_ptr->run ();

  // Shut down the OA -- recycles all underlying resources (e.g. file
  // descriptors, etc).

  //  oa_ptr->clean_shutdown (env);

  // Need to clean up and do a CORBA::release on everything we've
  // created!
  for (i = 0; i < num_of_objs_; i++)
    {
      delete my_cubit[i];
    }

  // Free resources
  CORBA::release (orb_ptr);
  CORBA::release (oa_ptr);
  }
  catch(const CORBA::SystemException& exc)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "CORBA::SystemException caught: %s (minor: %d): svr.cpp\n\n",
		  exc._repository_id (),
		  exc.minor ()));
    }
  catch(const CORBA::UserException& exc)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "CORBA::UserException caught: %s: svr.cpp\n\n",
		  exc._repository_id ()));
    }
  catch(...)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "Exception caught: svr.cpp\n\n"));
    }

  return 0;
}

static char *hostname = NULL;
static int base_port = 0;

// Parses the command line arguments and returns an error status

static int num_of_objs = 1;

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "h:p:n:");
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
      case 'n':
	num_of_objs = ACE_OS::atoi (opts.optarg);
	break;
      case '?':
      default:
	ACE_ERROR_RETURN ((LM_ERROR,
			   "usage:  %s"
			   " -p port"
			   " -h my_hostname"
                           " -n num_objects"
			   "\n", argv [0]), 1);
      }

  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
main (int argc, char *argv[])
{

#if 0
  parse_args (argc, argv);

  if ((hostname == NULL) || (base_port == 0)) 
    {
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "usage: %s "
			 " -p port -h my_hostname"
			 "\n",argv [0]), 1);
      
    }
#endif
  
  const size_t arg_size =
    strlen("rate20 -ORBport %d -ORBhost %s")
    + 5 /* port number */
    + MAXHOSTNAMELEN /* hostname */
    + 1 /* zero terminator */;
  
  char *args1 = 0;
  ACE_NEW_RETURN (args1, char[arg_size], -1);

  ::sprintf (args1, "rate20");
  Cubit_Task task1 (args1 , "internet", 1);
  ACE_Sched_Priority priority;
#if defined(VXWORKS)
  priority = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, ACE_SCOPE_THREAD) + 6;
#else
  priority = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);
#endif  
  ACE_DEBUG ((LM_DEBUG, "Creating servant with high priority\n"));
  task1.activate (THR_BOUND, 1, 0, priority);


  char *args2 = 0;
  ACE_NEW_RETURN (args2, char[arg_size], -1);

  ::sprintf (args2, "rate10");//, base_port+1, hostname);
  Cubit_Task task2 (args2 , "internet", 1);
  priority =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);
  ACE_DEBUG ((LM_DEBUG, "Creating servant with low priority\n"));
  task2.activate (THR_BOUND, 1, 0, priority);

  ACE_Thread_Manager::instance ()->wait ();

  delete [] args1;
  delete [] args2;
  
  return 0;
}
