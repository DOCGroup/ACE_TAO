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

#include "corba/orb.h"
#include "corba/debug.h"
#include "connect.h"

#include "cubit_i.h"

extern void
print_exception (const CORBA_Exception *, const char *, FILE *f=stdout);

//
// Standard command line parsing utilities used.
//
int
main (int    argc, char   *argv[])
{
  CORBA_Environment	env;
  CORBA_Object_ptr obj = 0;
  CORBA_ORB_ptr	orb_ptr;
  CORBA_BOA_ptr   oa_ptr;
  CORBA_String	key = (CORBA_String) "key0";
  char		*orb_name = "internet";
  int			idle = -1;
  int			debug_level = 1;

  orb_ptr = CORBA_ORB_init (argc, argv, orb_name, env);
  if (env.exception () != 0) {
    print_exception (env.exception (), "ORB init");
    return 1;
  }

  // Initialize the Basic Object Adapter
  oa_ptr = orb_ptr->BOA_init(argc, argv, "ROA");
  if (oa_ptr == 0)
    ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) Unable to initialize the BOA.\n"), 1);
  

  // Create implementation object with user specified key
  Cubit_i_ptr  my_cubit = new Cubit_i(key);

  if (debug_level >= 1)
    {
      // Why are we getting the BOA_ptr from here when we've already
      // got it above?
      CORBA_OctetSeq	obj_key;
      obj_key.buffer = (CORBA_Octet *) key;
      obj_key.length = obj_key.maximum = ACE_OS::strlen ((char *)key);
     
      if (oa_ptr->find(obj_key, obj) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) Unable to locate object with key '%s', %p\n", key), 2);
     
      //
      // Stringify the objref we'll be implementing, and
      // print it to stdout.  Someone will take that string
      // and give it to some client.  Then release the object.
      //
      CORBA_String	str;
     
      str = orb_ptr->object_to_string (obj, env);
      if (env.exception () != 0) {
        print_exception (env.exception (), "object2string");
        return 1;
      }
      ACE_OS::puts ((char *)str);
      ACE_OS::fflush (stdout);
      dmsg1 ("listening as object '%s'", str);
    }

  // Handle requests for this object until we're killed, or one of
  // the methods asks us to exit.
  //
  int terminationStatus = 0;

  // Insert Object Key into context
  //  params->context(&obj_key);

#if !defined(USE_HOMEBREW_EVENT_LOOP)
  ACE_Service_Config::run_reactor_event_loop();
#else
  while (1)
    {
      int result = params->reactor()->handle_events ();

      if (result == -1)
	{
	  terminationStatus = -1;
	  break;
	}
    }
#endif

  //
  // Shut down the OA -- recycles all underlying resources (e.g. file
  // descriptors, etc).
  //
  oa_ptr->clean_shutdown (env);
  return 0;
}
