// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Naming_Service
//
// = FILENAME
//    Naming_Utils.cpp
//
// = DESCRIPTION
//      This class implements a Naming Server wrapper class that holds
//      a Naming Context for TAO NameService.
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//    Matthew Braun <mjb2@cec.wustl.edu>
//
// ============================================================================

#include "orbsvcs/CosNamingC.h"
#include "tao/corba.h"
#include "Naming_Utils.h"
#include "ace/Arg_Shifter.h"

ACE_RCSID(Naming, Naming_Utils, "$Id$")

  // Default constructor

  TAO_Naming_Server::TAO_Naming_Server (void)
    : ior_multicast_ (0),
      naming_context_name_ (CORBA::string_dup ("NameService"))
{
}

// Constructor which takes an ORB and POA.

TAO_Naming_Server::TAO_Naming_Server (CORBA::ORB_ptr orb,
                                      PortableServer::POA_ptr child_poa,
				      int argc,
                                      char **argv)
{
  if (this->init (orb, child_poa, argc, argv) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "TAO_Naming_Server::init"));
}


// Function to locate (or create if necessary) a name service under
// the passed ORB and POA
int
TAO_Naming_Server::init (CORBA::ORB_ptr orb,
			 PortableServer::POA_ptr child_poa,
			 int argc,
			 char **argv)
{
  CORBA::Object_var naming_obj =
    orb->resolve_initial_references ("NameService");
  
  if (CORBA::is_nil (naming_obj.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
		    "\nNameService not found; calling init\n"));
      return (this->init_new_naming (orb, child_poa, argc, argv));
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nNameService found!\n")); 
      
      if (this->naming_context_impl_.init () == -1)
        return -1;
      TAO_TRY
        {
          this->naming_context_ =
            CosNaming::NamingContext::_narrow (naming_obj.in (),
                                               TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CORBA::String_var str =
            orb->object_to_string (naming_obj.in (),
                                   TAO_TRY_ENV);
	  
          TAO_CHECK_ENV;
	      
          this->naming_service_ior_ = str.in ();
          this->naming_context_name_ = CORBA::string_dup (""); 
        }
      TAO_ENDTRY;
      
      char* naming_ior = ACE_OS::strdup (this->naming_service_ior_.in ());
      u_short port = TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();

      ACE_NEW_RETURN (this->ior_multicast_,
                      TAO_IOR_Multicast (naming_ior,
                                         port,
                                         ACE_DEFAULT_MULTICAST_ADDR,
                                         TAO_SERVICEID_NAMESERVICE),
                      -1);
    }
  return 0;
}
	    
// Function to initialize the name server object under the passed ORB
// and POA.
int
TAO_Naming_Server::init_new_naming (CORBA::ORB_ptr orb,
				    PortableServer::POA_ptr child_poa,
				    int argc,
				    char **argv)
{
  // Initialize our <NS_NamingContext> instance.
  if (this->naming_context_impl_.init () == -1)
    return -1;

  // Parse command-line arguments to determine if this name server
  // instance is part of a naming tree that resides under the default
  // name server.
  ACE_Arg_Shifter args (argc, argv);

  while (args.is_anything_left ())
    {
      char *current_arg = args.get_current ();

      if (ACE_OS::strcmp (current_arg, "-NScontextname") == 0)
	{
	  args.consume_arg ();

	  if (args.is_parameter_next ())
	    {
	      this->naming_context_name_ =
		CORBA::string_dup (args.get_current ());
	      args.consume_arg ();
	    }
	}
      else
	args.ignore_arg ();
    }

  TAO_TRY
    {
      // Check if this invocation is creating a naming context
      // different from the default NameService context, if not,
      // instantiate a name service, and listen on multicast port.

      if (ACE_OS::strcmp (this->naming_context_name_.in (),
                          "NameService") == 0)
	{
          // Get the naming context ptr to NameService.
          this->naming_context_ =
            this->naming_context_impl_._this (TAO_TRY_ENV);
          TAO_CHECK_ENV;

	  PortableServer::ObjectId_var id =
	    PortableServer::string_to_ObjectId ("NameService");

	  child_poa->activate_object_with_id (id.in (),
					      &this->naming_context_impl_,
					      TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  // Stringify the objref we'll be implementing, and print it
	  // to stdout.  Someone will take that string and give it to
	  // a client.  Then release the object.
	  CORBA::Object_var obj =
	    child_poa->id_to_reference (id.in (),
					TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  this->naming_service_ior_=
	    orb->object_to_string (obj.in (),
				   TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  if (TAO_debug_level > 0)
	    ACE_DEBUG ((LM_DEBUG,
			"NameService IOR is <%s>\n",
			this->naming_service_ior_.in ()));
	  
#if defined (ACE_HAS_IP_MULTICAST)
	  // Get reactor instance from TAO.
	  ACE_Reactor *reactor =
	    TAO_ORB_Core_instance ()->reactor ();
	  
	  // First, see if the user has given us a multicast port number
	  // for the name service on the command-line;
	  u_short port =
	    TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();

	  if (port == 0)
	    {
	      const char *port_number =
		ACE_OS::getenv ("NameServicePort");

	      if (port_number != 0)
		port = ACE_OS::atoi (port_number);
	    }

	  if (port == 0)
	    port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;

	  char *naming_ior = ACE_OS::strdup (this->naming_service_ior_.in ());

	  // Instantiate a server which will receive requests for an ior
	  ACE_NEW_RETURN (this->ior_multicast_,
			  TAO_IOR_Multicast (naming_ior,
					     port,
					     ACE_DEFAULT_MULTICAST_ADDR,
					     TAO_SERVICEID_NAMESERVICE),
			  -1);

	  // Register event handler for the ior multicast.
	  if (reactor->register_handler (this->ior_multicast_,
					 ACE_Event_Handler::READ_MASK) == -1)
	    {
	      if (TAO_debug_level > 0)
		ACE_DEBUG ((LM_DEBUG,
			    "cannot register Event handler\n"));
	    }
	  else
	    {
	      if (TAO_debug_level > 0)
		ACE_DEBUG ((LM_DEBUG,
			    "The multicast server setup is done.\n"));
	    }
#endif /* ACE_HAS_IP_MULTICAST */
	}
      else
	{
	  // We are instantiating a name service to manage a naming
	  // context other than the default NameService context.  We
	  // need to verify that all contexts in the compound name
	  // exist (if we are passed a compound naming context).  Any
	  // missing contexts will be created and bound into the
	  // naming service utilized by this name server instance.
	  // Finally, this name server will bind the context that it's
	  // managing into it's parent's context.

	  CORBA::Object_var default_ns =
	    orb->resolve_initial_references ("NameService");

	  if (CORBA::is_nil (default_ns.in ()))
	    ACE_ERROR ((LM_ERROR,
                        "%p\n",
                        "NameService"));

	  CosNaming::NamingContext_var default_ctx =
	    CosNaming::NamingContext::_narrow (default_ns.in (),
                                               TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  // Temporary hack, to see if this works as expected.  Need
	  // to add code to walk the naming tree from root to the
	  // context represented by this server.
	  PortableServer::ObjectId_var id =
	    PortableServer::string_to_ObjectId (this->naming_context_name_.in ());

	  child_poa->activate_object_with_id (id.in (),
					      &naming_context_impl_,
					      TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  // Stringify the objref we'll be implementing, and print it to
	  // stdout.  Someone will take that string and give it to a
	  // client.  Then release the object.
	  CORBA::Object_var obj =
	    child_poa->id_to_reference (id.in (),
					TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  this->naming_service_ior_=
	    orb->object_to_string (obj.in (),
				   TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  if (TAO_debug_level > 0)
	    ACE_DEBUG ((LM_DEBUG,
			"NameService IOR is <%s>\n",
			this->naming_service_ior_.in ()));

	  // Bind our context into the default name server.
	  CosNaming::Name ctx_name (1);
	  ctx_name.length (1);
	  ctx_name[0].id = CORBA::string_dup (naming_context_name_.in ());

	  default_ctx->bind_context (ctx_name,
                                     naming_context_impl_._this (TAO_TRY_ENV),
				     TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Naming Service");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// Returns the "NameService" NamingContext implementation object.

NS_NamingContext &
TAO_Naming_Server::get_naming_context (void)
{
  return naming_context_impl_ ;
}

CORBA::String
TAO_Naming_Server::naming_service_ior (void)
{
  return CORBA::string_dup (this->naming_service_ior_.in ());
}

// Returns a pointer to the NamingContext.

CosNaming::NamingContext_ptr
TAO_Naming_Server::operator -> (void) const
{
  return this->naming_context_.ptr ();
}

// Destructor.

TAO_Naming_Server::~TAO_Naming_Server (void)
{
  if (this->ior_multicast_ != 0)
    {
      TAO_ORB_Core_instance ()->reactor ()->remove_handler (this->ior_multicast_,
                                                            ACE_Event_Handler::READ_MASK);
      delete this->ior_multicast_;
    }
}

// Returns a pointer to the NamingContext.

CosNaming::NamingContext_ptr
TAO_Naming_Client::operator -> (void) const
{
  return this->naming_context_.ptr ();
}

// Returns a pointer to the NamingContext.
CosNaming::NamingContext_ptr
TAO_Naming_Client::get_context (void) const
{
  return this->naming_context_.ptr ();
}

int 
TAO_Naming_Client::init (CORBA::ORB_ptr orb,
                         int argc,
                         char **argv)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
	orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the NameService.\n"),
			  -1);
      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in (), 
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

TAO_Naming_Client::TAO_Naming_Client (void)
{
  // Do nothing
}

TAO_Naming_Client::~TAO_Naming_Client (void)
{
  // Do nothing
}
