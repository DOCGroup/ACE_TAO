// $Id$

#include "MCastC.h"
#include "MCastS.h"
#include "server_i.h"
#include "MCast_Server_i.h"

#include "tao/ORB_Core.h"
#include "tao/IORTable/IORTable.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// Constructor
Server_i::Server_i (void)
{
}

// Destructor
Server_i::~Server_i (void)
{
}

int
Server_i::init (int &argc,
                char **&argv
                TAO_ENV_ARG_DECL)
{
  this->argc_ = argc;
  this->argv_ = argv;

  ACE_TRY
    {
      // First initialize the ORB, that will remove some arguments...
      this->orb_ =
        CORBA::ORB_init (this->argc_,
                         this->argv_,
                         "" /* the ORB name, it can be anything! */
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get a reference to the RootPOA.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow down to the correct reference.
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set a POA Manager.
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate the POA Manager.
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior;

      // Create the servant
      MCast_Server_i server_i;

      // Activate it to obtain the reference
      MCast::Server_var mcast_server =
        server_i._this ();

      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          ior =
            this->orb_->object_to_string (mcast_server.in ()
                                          TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          adapter->bind ("MCASTServer", ior.in () TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Enable such that the server can listen for multicast requests
      // at the specified address.
      this->enable_multicast (ior.in ()
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Run the ORB
      this->orb_->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //Destroy the POA, waiting until the destruction terminates.
      poa->destroy (1, 1);
      this->orb_->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Server_i::enable_multicast (const char *ior
                            TAO_ENV_ARG_DECL_NOT_USED)
{

  ACE_TRY
    {
      this->parse_args (this->argc_, this->argv_);

      // Get reactor instance from TAO.
      ACE_Reactor *reactor =
        this->orb_->orb_core ()->reactor ();

      // Instantiate a handler which will handle client requests for
      // the bottstrappable service, received on the multicast port.
      ACE_NEW_RETURN (this->ior_multicast_,
                      TAO_IOR_Multicast (),
                      -1);

      if (this->ior_multicast_->init (ior,
                                      this->mcast_address_.in (),
                                      TAO_SERVICEID_MCASTSERVER) == -1)
        return -1;

      // Register event handler for the ior multicast.
      if (reactor->register_handler (this->ior_multicast_,
                                     ACE_Event_Handler::READ_MASK) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "MCast_Server: cannot register Event handler\n"));
          return -1;
        }

    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      //
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
Server_i::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opts (argc, argv, "a:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        this->mcast_address_ = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <mcast_address>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
