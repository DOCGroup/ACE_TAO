// $Id$

#include "MCastC.h"
#include "MCastS.h"
#include "server_i.h"
#include "MCast_Server_i.h"

#include "tao/ORB_Core.h"
#include "tao/IORTable/IORTable.h"
#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

Server_i::Server_i (void)
  : argc_ (0),
    argv_ (0),
    orb_ (),
    ior_multicast_ (0),
    service_ior_ (),
    mcast_address_ ()
{
}

Server_i::~Server_i (void)
{
  delete this->ior_multicast_;
}

int
Server_i::init (int &argc,
                ACE_TCHAR **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // First initialize the ORB, that will remove some arguments...
      this->orb_ =
        CORBA::ORB_init (this->argc_, this->argv_);

      // Get a reference to the RootPOA.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      // Narrow down to the correct reference.
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in ());

      // Set a POA Manager.
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();

      // Activate the POA Manager.
      poa_manager->activate ();

      CORBA::String_var ior;

      // Create the servant
      MCast_Server_i server_i;

      // Activate it to obtain the reference
      MCast::Server_var mcast_server =
        server_i._this ();

      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          ior =
            this->orb_->object_to_string (mcast_server.in ());
          adapter->bind ("MCASTServer", ior.in ());
        }

      // Enable such that the server can listen for multicast requests
      // at the specified address.
      if (this->enable_multicast (ior.in ()) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Unable to enable multicast "
                      "on specified address.\n"));

          throw CORBA::INTERNAL ();
        }

      // Run the ORB
      this->orb_->run ();

      //Destroy the POA, waiting until the destruction terminates.
      poa->destroy (1, 1);
      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
      return 1;
    }

  return 0;
}

int
Server_i::enable_multicast (const char *ior)
{
  if (this->parse_args (this->argc_, this->argv_) != 0)
    return -1;

  // Get reactor instance from TAO.
  ACE_Reactor *reactor =
    this->orb_->orb_core ()->reactor ();

  // Instantiate a handler which will handle client requests for the
  // bootstrappable service, received on the multicast port.
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

  return 0;
}

int
Server_i::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        this->mcast_address_ = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
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
  // Indicates successful parsing of the command line
  return 0;
}
