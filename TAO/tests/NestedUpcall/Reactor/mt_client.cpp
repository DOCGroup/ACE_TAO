// $Id$

#include "mt_client.h"
#include "eh_i.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(Reactor, client, "$Id$")

// Constructor.
NestedUpCalls_MT_Client::NestedUpCalls_MT_Client (void)
  : nested_up_calls_reactor_key_ ("file://test.ior"),
    shutdown_ (0),
    call_count_ (5),
    nthreads_ (4)
{
}

int
NestedUpCalls_MT_Client::parse_args (int argc, char* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dxn:t:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;

      case 'x':
        this->shutdown_ = 1;
        break;

      case 'n':
        this->call_count_ = ACE_OS::atoi (get_opts.optarg);
        break;

      case 't':
        this->nthreads_ = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'k': // read the nestedupcalls IOR from the command-line.
        this->nested_up_calls_reactor_key_ = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-k nestedupcalls-obj-ref-key]"
                           " [-n call count]"
                           " [-t thread count]"
                           " [-x]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
NestedUpCalls_MT_Client::run (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG, "Activating <%d> threads (%P|%t)\n",
                  this->nthreads_));
      this->activate (THR_NEW_LWP|THR_JOINABLE,
                      this->nthreads_);

      ACE_Thread_Manager::instance ()->wait ();

      if (this->shutdown_)
        {
          this->reactor_->stop (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Nestedupcalls::run ()");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

// Execute client example code.
int
NestedUpCalls_MT_Client::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG, "Running thread (%t)\n"));

      // Create an EventHandler servant to hand to the other side...
      auto_ptr<EventHandler_i> eh_impl (new EventHandler_i);
      EventHandler_var eh = eh_impl->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now, we can invoke an operation on the remote side.
      CORBA::Long r =
        this->reactor_->register_handler (eh.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // We ought to have a result!
      ACE_DEBUG ((LM_DEBUG,
                  "received %d as return from register_handler ()\n",
                  r));

      this->reactor_->set_value (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->reactor_->decrement (eh.in (),
                                 this->call_count_,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Nestedupcalls::svc ()");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

NestedUpCalls_MT_Client::~NestedUpCalls_MT_Client (void)
{
}

int
NestedUpCalls_MT_Client::init (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (argc, argv,
                                    "",
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args (argc, argv) == -1)
        return -1;

      CORBA::Object_var reactor_object =
        this->orb_->string_to_object (this->nested_up_calls_reactor_key_,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->reactor_ =
        Reactor::_narrow (reactor_object.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->reactor_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid reactor key <%s>\n",
                           this->nested_up_calls_reactor_key_),
                          -1);


      ACE_DEBUG ((LM_DEBUG, "Reactor received OK\n"));

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NestedUpCalls::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// This function runs the test.
int
main (int argc, char *argv[])
{
  NestedUpCalls_MT_Client nestedupcalls_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls: client \n\n"));

  if (nestedupcalls_client.init (argc, argv) == -1)
    return 1;

  return nestedupcalls_client.run ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<EventHandler_i>;
template class ACE_Auto_Basic_Ptr<EventHandler_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<EventHandler_i>
#pragma instantiate ACE_Auto_Basic_Ptr<EventHandler_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
