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

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

#include "tao/corba.h"

#include "cubit_i.h"

// @@ Shouldn't this be defined in a headerfile somewhere?
extern void
print_exception (const CORBA::Exception *, const char *, FILE *f=stdout);

// Global Variables
static CORBA::String key = (CORBA::String) "key0";

static int num_of_objs = 1;

// Parses the command line arguments and returns an error status

static int 
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "dk:n:");
  int			c;
   
  while ((c = opts ()) != -1)
    switch (c) 
      {
      case 'd':  // debug flag
	TAO_debug_level++;
	break;
      case 'k':			// key (str)
	key = (CORBA::String) opts.optarg;
	break;
      case 'n':			// idle seconds b4 exit
	num_of_objs = ACE_OS::atoi (opts.optarg);
	break;
      case '?':
      default:
	ACE_ERROR_RETURN ((LM_ERROR,
			   "usage:  %s"
			   " [-d]"
			   " [-k {object_key}]"
			   "\n", argv [0]), 1);
      }
   
  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
main (int argc, char *argv[])
{
  CORBA::Environment env;
  char *orb_name = "internet";

  CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc, argv, orb_name, env);

  if (env.exception () != 0) 
    {
      print_exception (env.exception (), "ORB init");
      return 1;
    }

  // Initialize the Basic Object Adapter
  CORBA::POA_ptr oa_ptr = orb_ptr->POA_init (argc, argv, "ROA");

  if (oa_ptr == 0)
    ACE_ERROR_RETURN ((LM_ERROR, " (%P|%t) Unable to initialize the POA.\n"), 1);

  // Parse remaining command line and verify parameters.

  parse_args (argc, argv);
  
  // Create implementation object with user specified key.
  Cubit_i_ptr *my_cubit = new Cubit_i_ptr[num_of_objs];

  for (int i = 0; i < num_of_objs; i++)
    {
      CORBA::String obj_str = CORBA::string_alloc (ACE_OS::strlen ((char *) key)+2);

      ::sprintf (obj_str, "%s%d", (char *) key, i);

      my_cubit[i] = new Cubit_i (obj_str);

      if (my_cubit[i] == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to create implementation object&d\n",
			   i),
			  2);

      if (TAO_debug_level >= 0)
	{
	  // Why are we getting the POA_ptr from here when we've
	  // already got it above?

	  CORBA::OctetSeq obj_key;
	  obj_key.buffer = (CORBA::Octet *) obj_str;
	  obj_key.length = obj_key.maximum = ACE_OS::strlen (obj_str);
     
	  CORBA::Object_ptr obj = 0;

#if 0
	  if (oa_ptr->find (obj_key, obj) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR,
			       " (%P|%t) Unable to locate object with key '%s', %p\n", 
			       key),
			      3);
#endif
     
 	  // Stringify the objref we'll be implementing, and print it
	  // to stdout.  Someone will take that string and give it to
	  // some client.  Then release the object.

	  CORBA::String str;
         
	  str = orb_ptr->object_to_string (my_cubit[i], env);

	  if (env.exception () != 0) 
	    {
	      print_exception (env.exception (), "object2string");
	      return 1;
	    }

	  ACE_OS::puts ((char *) str);
	  ACE_OS::fflush (stdout);
	  dmsg1 ("Object Created at: '%ul'", obj);
	  dmsg1 ("listening as object '%s'", str);
	}

      CORBA::string_free (obj_str);  
    }

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  orb_ptr->run ();

  // Shut down the OA -- recycles all underlying resources (e.g. file
  // descriptors, etc).

  oa_ptr->clean_shutdown (env);
  return 0;
}
