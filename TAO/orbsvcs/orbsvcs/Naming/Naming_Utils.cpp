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
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#include "orbsvcs/CosNamingC.h"
#include "tao/corba.h"
#include "Naming_Utils.h"

// Default constructor

TAO_Naming_Server::TAO_Naming_Server (void)
  :  ior_multicast_ (0)
{
}

// Constructor which takes an ORB and POA.

TAO_Naming_Server::TAO_Naming_Server (CORBA::ORB_ptr orb,
                                      PortableServer::POA_ptr child_poa)
{
  this->init (orb, child_poa);
}

// Function to initialize the name server object under the passed orb
// and poa

int
TAO_Naming_Server::init (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr child_poa)
{
  TAO_TRY
    {
      // Get the naming context ptr to NameService.
      this->naming_context =
        naming_context_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("NameService");

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

      ACE_DEBUG ((LM_DEBUG,
                  "listening as object <%s>\n",
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
        ACE_DEBUG ((LM_DEBUG,
                    "cannot register Event handler\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "The multicast server setup is done.\n"));
#endif /* ACE_HAS_IP_MULTICAST */
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
TAO_Naming_Server::GetNamingContext (void)
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
  // @@ Is the memory managment here correct? Is it clear?
  return this->naming_context.ptr ();
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
