// $Id$

#include "orbsvcs/CosNamingC.h"
#include "tao/corba.h"
#include "Naming_Utils.h"
#include "ace/Arg_Shifter.h"
#include "Hash_Naming_Context.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(Naming, Naming_Utils, "$Id$")

TAO_Naming_Server::TAO_Naming_Server (void)
  : naming_context_impl_ (0),
    naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ ()
{
}

TAO_Naming_Server::TAO_Naming_Server (CORBA::ORB_ptr orb,
                                      PortableServer::POA_ptr poa,
                                      size_t context_size,
                                      ACE_Time_Value *timeout,
                                      int resolve_for_existing_naming_service)
  : naming_context_impl_ (0),
    naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ ()
{
  if (this->init (orb,
                  poa,
                  context_size,
                  timeout,
                  resolve_for_existing_naming_service) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "TAO_Naming_Server::init"));
}


int
TAO_Naming_Server::init (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         size_t context_size,
                         ACE_Time_Value *timeout,
                         int resolve_for_existing_naming_service)
{
  CORBA::Object_var naming_obj;

  if (resolve_for_existing_naming_service)
    naming_obj = orb->resolve_initial_references ("NameService", timeout);

  if (CORBA::is_nil (naming_obj.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nNameService not resolved, so we'll become a NameService\n"));
      return this->init_new_naming (orb,
                                    poa,
                                    context_size);
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nNameService found!\n"));

      TAO_TRY
        {
          this->naming_context_ =
            CosNaming::NamingContext::_narrow (naming_obj.in (),
                                               TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->naming_service_ior_ =
            orb->object_to_string (naming_obj.in (),
                                   TAO_TRY_ENV);

          TAO_CHECK_ENV;
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("TAO_Naming_Server::init");
        }
      TAO_ENDTRY;
    }
  return 0;
}

int
TAO_Naming_Server::init_new_naming (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr poa,
                                    size_t context_size)
{
  TAO_Naming_Context *c = 0;
  TAO_Hash_Naming_Context *c_impl = 0;

  ACE_NEW_RETURN (c_impl,
                  TAO_Hash_Naming_Context (poa,
                                           "NameService",
                                           context_size,
                                           1),
                  -1);
  // Put c_impl into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Hash_Naming_Context> impl_temp (c_impl);

  ACE_NEW_RETURN (c,
                  TAO_Naming_Context (c_impl),
                  -1);
  // Allocation succeeded, get rid of auto pointer.
  impl_temp.release ();

  // Set implementation's pointer to it's abstraction.
  c_impl->interface (c);

  // Now wrap this pointer into Auto_Ptr until everything succeeds.
  ACE_Auto_Basic_Ptr<TAO_Naming_Context> c_temp (c);

  TAO_TRY
    {
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("NameService");

      poa->activate_object_with_id (id.in (),
                                    c,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->naming_context_ =
        c->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.
      this->naming_service_ior_=
        orb->object_to_string (this->naming_context_.in (),
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "NameService IOR is <%s>\n",
                    this->naming_service_ior_.in ()));

      // everything succeeded, so set the pointer, get rid of Auto_Ptr.
      this->naming_context_impl_ = c_temp.release ();

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

      // Instantiate a server which will receive requests for an ior
      ACE_NEW_RETURN (this->ior_multicast_,
                      TAO_IOR_Multicast (this->naming_service_ior_.in (),
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
          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "The multicast server setup is done.\n"));
        }
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

TAO_Naming_Context &
TAO_Naming_Server::get_naming_context (void)
{
  return *this->naming_context_impl_;
}

CORBA::String
TAO_Naming_Server::naming_service_ior (void)
{
  return CORBA::string_dup (this->naming_service_ior_.in ());
}

// Returns a pointer to the NamingContext.

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
  delete this->naming_context_impl_;
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
                         ACE_Time_Value *timeout)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService", timeout);

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Auto_Basic_Ptr<TAO_Naming_Context>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
  #pragma instantiate ACE_Auto_Basic_Ptr<TAO_Naming_Context>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
