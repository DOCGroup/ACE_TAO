//#include "NS_CosNaming.h"
#include "CosNaming_i.h"
#include <iostream.h>

// This is a startup for the naming server.
// This is used for testing of the Naming Service.

int
main (int argc, char ** argv)
{
  CORBA::Environment env;
  char *orb_name = "nameserver";

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

  // create an Naming Service object

  //  CosNaming_i * name_server = new CosNaming_i;
NS_NamingContext *naming_context = new NS_NamingContext;
  
  // Stringify the objref we'll be implementing, and print it to
  // stdout.  Someone will take that string and give it to a
  // client.  Then release the object.

  CORBA::String str;

  str = orb_ptr->object_to_string (naming_context, env);

  if (env.exception () != 0)
    {
      env.print_exception ("object2string");
      return 1;
    }

  ACE_OS::puts ((char *) str);
  ACE_OS::fflush (stdout);
  dmsg1 ("listening as object '%s'", str);

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  if (orb_ptr->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  return 0;
}


//  :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#if 0
  NS_NamingContext *c = new NS_NamingContext;

  CosNaming::NamingContext_ptr cont = new TIE_CosNaming_NamingContext 
    (NS_NamingContext) (c);
    
  c->initialize (cont);

  try 
    {
      CORBA::Orbix.impl_is_ready ("nameserver", 30000);
				  //CORBA::Orbix.INFINITE_TIMEOUT);
      // The server will not time out unless something goes wrong.
    }

  catch (const CORBA::SystemException& se)
    {
      cout << "Unexpected exception: " << endl << &se;
      return -1;
    }

  return 0;
}
#endif
