// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/bin/Logger
//
// = FILENAME
//    svr.cpp
//
// = DESCRIPTION 
//    This program is an implementation of a simple logger service.  
//    Whatever is sent to it through its interface is displayed on stdout.
//    It uses the Logger_Factory server to create a number of logger objects.  
//
// = AUTHORS
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include <iostream.h>
#include "loggerC.h"
#include "logger_i.h"
#include "ior_multicast.h"

int
main (int argc, char ** argv)
{
  CORBA::Environment env;
  char *orb_name = "logger";

  CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc, argv, orb_name, env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB init");
      return 1;
    }

  // Initialize the Object Adapter
  CORBA::POA_ptr oa_ptr = orb_ptr->POA_init (argc, argv, "POA");

  if (oa_ptr == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize the POA.\n"),
		      1);

  // create a factory implementation
  Logger_Factory_ptr f;

  ACE_NEW_RETURN (f, Logger_Factory_i ("factory"), -1);

  // Stringify the objref we'll be implementing, and print it to
  // stdout.  Someone will take that string and give it to a
  // client.  Then release the object.
  
  CORBA::String str;
  
  str = orb_ptr->object_to_string (f, env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("object2string");
      return 1;
    }
  
  ACE_OS::puts ((char *) str);
  ACE_OS::fflush (stdout);
  dmsg1 ("listening as object '%s'", str);

  // Use IP multicast to advertise the service
#if defined (ACE_HAS_IP_MULTICAST)
  // get reactor instance from TAO
  ACE_Reactor *reactor = TAO_ORB_Core_instance()->reactor();
  
  // Instantiate a server which will receive requests for an ior
  IOR_Multicast ior_multicast (str,
			       DEFAULT_LOGGER_SERVER_REQUEST_PORT, 
			       DEFAULT_LOGGER_SERVER_MULTICAST_ADDR,
			       DEFAULT_LOGGER_SERVER_REPLY_PORT); 

  // register event handler for the ior multicast.
  if (reactor->register_handler (&ior_multicast,
				 ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "register_handler", 1));
    
  ACE_DEBUG ((LM_DEBUG, "The multicast server setup is done.\n"));
#endif /* ACE_HAS_IP_MULTICAST */

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  if (orb_ptr->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);
  
  cout << "Server logger_factory is terminating" << endl;
  return 0;
}

