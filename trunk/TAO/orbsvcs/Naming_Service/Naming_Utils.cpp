// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Naming_Service
//
// = FILENAME
//    Naming_Server.cpp
//
// = DESCRIPTION
//      This class implements a Naming Server wrapper class which holds
//      a Naming Context for TAO NameService.
//
// = AUTHOR
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#include "ace/streams.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/corba.h"
#include "Naming_Server.h"

// Default constructor

TAO_Naming_Server::TAO_Naming_Server ()
  :ior_multicast_ (0),
   naming_context_ptr_ (0)
{
}

// Constructor which takes an orb and poa

TAO_Naming_Server::TAO_Naming_Server (CORBA::ORB_var& orb,PortableServer::POA_var& good_poa)
{
  this->init (orb,good_poa);
}

// Function to initialize the name server object under the passed orb
// and poa

int
TAO_Naming_Server::init  (CORBA::ORB_var& orb,PortableServer::POA_var& good_poa)
{
  TAO_TRY
    {
      // get the naming context ptr to NameService
      naming_context_ptr_ = naming_context_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::ObjectId_var id = 
	PortableServer::string_to_ObjectId ("NameService");
      good_poa->activate_object_with_id (id.in (),
					 &naming_context_impl_,
					 TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.
      CORBA::Object_var obj = 
	good_poa->id_to_reference (id.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
	orb->object_to_string (obj.in (),
			       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "listening as object <%s>\n", str.in ()));

#if defined (ACE_HAS_IP_MULTICAST)
      // get reactor instance from TAO
      ACE_Reactor *reactor = TAO_ORB_Core_instance ()->reactor ();
  
      // First, see if the user has given us a multicast port number
      // for the name service on the command-line;
      u_short port = TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();
      if (port == 0)
	{
	  const char *port_number = ACE_OS::getenv ("NameServicePort");

	  if (port_number != 0)
	    port = ACE_OS::atoi (port_number);
	}

      if (port == 0)
	port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;


      char *naming_ior = ACE_OS::strdup (str.in ());
      // Instantiate a server which will receive requests for an ior
      ior_multicast_= new IOR_Multicast (naming_ior ,
				   port,
				   ACE_DEFAULT_MULTICAST_ADDR);

      // register event handler for the ior multicast.
      if (reactor->register_handler (ior_multicast_,
				     ACE_Event_Handler::READ_MASK) == -1)
	ACE_DEBUG ((LM_DEBUG, "cannot register Event handler\n"));
      else
	ACE_DEBUG ((LM_DEBUG, "The multicast server setup is done.\n"));
#endif /* ACE_HAS_IP_MULTICAST */
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Naming Service");
    }
  TAO_ENDTRY;
  return 0;
}

// Returns the "NameService" NamingContext implementation object

NS_NamingContext& TAO_Naming_Server::GetNamingContext ()
{
  return naming_context_impl_ ;
}

// Returns a pointer to the NamingContext

 CosNaming::NamingContext*
TAO_Naming_Server::operator -> (void) const
{
  return this->naming_context_ptr_;
}

//Destructor

TAO_Naming_Server::~TAO_Naming_Server ()
{
  delete ior_multicast_;
}


