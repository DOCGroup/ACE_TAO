// $Id$

#include "orbsvcs/CosNamingC.h"
#include "tao/corba.h"
#include "Naming_Utils.h"
#include "ace/Arg_Shifter.h"
#include "Transient_Naming_Context.h"
#include "Persistent_Context_Index.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(Naming, Naming_Utils, "$Id$")

TAO_Naming_Server::TAO_Naming_Server (void)
  : naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ (),
    context_index_ (0)
{
}

TAO_Naming_Server::TAO_Naming_Server (CORBA::ORB_ptr orb,
                                      PortableServer::POA_ptr poa,
                                      size_t context_size,
                                      ACE_Time_Value *timeout,
                                      int resolve_for_existing_naming_service,
                                      LPCTSTR persistence_location)
  : naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ (),
    context_index_ (0)
{
  if (this->init (orb,
                  poa,
                  context_size,
                  timeout,
                  resolve_for_existing_naming_service,
                  persistence_location) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "TAO_Naming_Server::init"));
}


int
TAO_Naming_Server::init (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         size_t context_size,
                         ACE_Time_Value *timeout,
                         int resolve_for_existing_naming_service,
                         LPCTSTR persistence_location)
{
  if (resolve_for_existing_naming_service)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          //
          // Try to find an existing Naming Service.
          //
          CORBA::Object_var naming_obj =
            orb->resolve_initial_references ("NameService",
                                             timeout,
                                             ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (!CORBA::is_nil (naming_obj.in ()))
            {
              //
              // Success in finding a Naming Service.
              //
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "\nNameService found!\n"));

              this->naming_context_ =
                CosNaming::NamingContext::_narrow (naming_obj.in (),
                                                   ACE_TRY_ENV);
              ACE_TRY_CHECK;

              this->naming_service_ior_ =
                orb->object_to_string (naming_obj.in (),
                                       ACE_TRY_ENV);
              ACE_TRY_CHECK;

              return 0;
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Server::init");
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nWe'll become a NameService\n"));

  // Become a Naming Service.
  return this->init_new_naming (orb,
                                poa,
                                persistence_location,
                                context_size);
}

int
TAO_Naming_Server::init_new_naming (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr poa,
                                    LPCTSTR persistence_location,
                                    size_t context_size)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (persistence_location != 0)
        //
        // Initialize Persistent Naming Service.
        //
        {
          // Allocate and initialize Persistent Context Index.
          ACE_NEW_RETURN (this->context_index_,
                          TAO_Persistent_Context_Index (orb, poa),
                          -1);

          if (this->context_index_->open (persistence_location) == -1
              || this->context_index_->init (context_size) == -1)
            {
              if (TAO_debug_level >0)
                ACE_DEBUG ((LM_DEBUG,
                            "TAO_Naming_Server: context_index initialization failed\n"));
              return -1;
            }

          // Set the root Naming Context reference.
          this->naming_context_ =
            this->context_index_->root_context ();
        }
      else
        {
          //
          // Initialize Transient Naming Service.
          //
          this->naming_context_ =
            TAO_Transient_Naming_Context::make_new_context (poa,
                                                            TAO_ROOT_NAMING_CONTEXT,
                                                            context_size,
                                                            ACE_TRY_ENV);
          ACE_TRY_CHECK;

        }

      // Set the ior of the root Naming Context.
      this->naming_service_ior_=
        orb->object_to_string (this->naming_context_.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Make the Naming Service locatable through iioploc.
      if (orb->_tao_add_to_IOR_table ("NameService",
                                      this->naming_context_.in ())
          == -1)
        {
          if (TAO_debug_level >0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_Naming_Server: cannot add to ior table.\n"));
          return -1;
        }
#if defined (ACE_HAS_IP_MULTICAST)
      //
      // Install ior multicast handler.
      //
      // Get reactor instance from TAO.
      ACE_Reactor *reactor =
        TAO_ORB_Core_instance ()->reactor ();

      // See if the -ORBMulticastDiscoveryEndpoint option was specified.
      ACE_CString mde (TAO_ORB_Core_instance ()->orb_params ()
                       ->mcast_discovery_endpoint ());

      // First, see if the user has given us a multicast port number
      // on the command-line;
      u_short port =
        TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();

      if (port == 0)
        {
          // Check environment var. for multicast port.
          const char *port_number =
            ACE_OS::getenv ("NameServicePort");

          if (port_number != 0)
            port = ACE_OS::atoi (port_number);
        }

      // Port wasn't specified on the command-line or in environment -
      // use the default.
      if (port == 0)
        port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;

      // Instantiate a handler which will handle client requests for
      // the root Naming Context ior, received on the multicast port.
      ACE_NEW_RETURN (this->ior_multicast_,
                      TAO_IOR_Multicast (),
                      -1);

      if (mde.length () != 0)
        {
          if (this->ior_multicast_->init (this->naming_service_ior_.in (),
                                          mde.c_str (),
                                          TAO_SERVICEID_NAMESERVICE) == -1)
            return -1;
        }
      else
        {
          if (this->ior_multicast_->init (this->naming_service_ior_.in (),
                                          port,
                                          ACE_DEFAULT_MULTICAST_ADDR,
                                          TAO_SERVICEID_NAMESERVICE) == -1)
            return -1;
        }

      // Register event handler for the ior multicast.
      if (reactor->register_handler (this->ior_multicast_,
                                     ACE_Event_Handler::READ_MASK) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_Naming_Server: cannot register Event handler\n"));
          return -1;
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Naming_Server: The multicast server setup is done.\n"));

#endif /* ACE_HAS_IP_MULTICAST */
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Server");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

char*
TAO_Naming_Server::naming_service_ior (void)
{
  return CORBA::string_dup (this->naming_service_ior_.in ());
}

CosNaming::NamingContext_ptr
TAO_Naming_Server::operator-> (void) const
{
  return this->naming_context_.ptr ();
}

TAO_Naming_Server::~TAO_Naming_Server (void)
{
  if (this->ior_multicast_ != 0)
    {
      TAO_ORB_Core_instance ()->reactor ()->remove_handler
        (this->ior_multicast_,
         ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
      delete this->ior_multicast_;
    }

  delete context_index_;
}

CosNaming::NamingContext_ptr
TAO_Naming_Client::operator -> (void) const
{
  return this->naming_context_.ptr ();
}

CosNaming::NamingContext_ptr
TAO_Naming_Client::get_context (void) const
{
  // Must duplicate this pointer so it can be stored in a _var.
  return CosNaming::NamingContext::_duplicate (this->naming_context_.ptr ());
}

int
TAO_Naming_Client::init (CORBA::ORB_ptr orb,
                         ACE_Time_Value *timeout)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService", timeout, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the NameService.\n"),
                          -1);
      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

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
