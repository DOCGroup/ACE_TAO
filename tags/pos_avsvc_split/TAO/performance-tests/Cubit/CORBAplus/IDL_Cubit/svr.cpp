// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Cubit/CORBAplus/IDL_Cubit
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
#include "pbroker/corba/orb.h"
#include "pbroker/corba/environ.h"
#include "pbroker/pberr.h"
#include <pbroker/corba/xpsorb.h>
#include <pbroker/corba/xpsboa.h>
#include "pbroker/corba/boad/boafull.h"   // will force usage of "Full" BOA API

#include <pbroker/unixsvc/unixsvc.h>

#include "cubit_i.h"

ACE_RCSID(IDL_Cubit, svr, "$Id$")

// Global Variables
static int num_of_objs = 1;
static pbbool killSelf = pbfalse;

// Declare the server activation policy
XPS_DECLARE_SERVER(Cubit_Factory,"Cubit_Factory",SHARED_SERVER)

// Parses the command line arguments and returns an error status

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dn:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        break;
      case 'n':	// number of cubit objects we hold
        num_of_objs = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of cubit objects>"
                           "\n", argv [0]), 1);
      }

  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
main (int argc, char *argv[])
{
  CORBA_Environment env;
  //  char *orb_name = "internet";
  
  XpsORB_ptr       orb_ptr = XpsORB::_nil();
  XpsBOA *         oa_ptr  = XpsBOA::_nil ();
  CORBA_Object_ptr obj_ptr = CORBA_Object::_nil();
  
  try
    {
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


      // Parse remaining command line and verify parameters.
      //      parse_args (argc, argv);

      // create a factory implementation
      Cubit_Factory_ptr factory;
      
      ACE_NEW_RETURN (factory, Cubit_Factory_i ("factory", num_of_objs), 1);
      
      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.
      
      CORBA::String str = 0;
      
      str = orb_ptr->object_to_url (factory);
      
      ACE_DEBUG ((LM_DEBUG, "listening as object '%s'\n", str));
      CORBA_string_free (str);

      oa_ptr->obj_is_ready (factory, nil);
      oa_ptr->registerAlias (factory, "cubit_factory");

      XpsEventService  eventService;
      eventService.mainloop();   // control will not return to this point
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

  // Free resources
  CORBA::release (oa_ptr);
  CORBA::release (orb_ptr);
  
  return 0;
}
