// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    client.cpp
//
// = AUTHOR
//    Chris Cleeland
//
// ============================================================================

#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"

#include "tao/corba.h"

#include "eh_i.h"

CORBA::String_var remote_reactor_key = (char *) 0;

// Completely arbitrary constant that ought to be big enough.
#define MAX_IOR_SIZE 4096

int
parse_args (int argc_, char* argv_[])
{
  ACE_Get_Opt get_opts (argc_, argv_, "df:k:");
  int c;
  char temp_buf[MAX_IOR_SIZE];
  char *result = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'f':
        {
          FILE *ior_file_ =
            ACE_OS::fopen (get_opts.optarg,"r");

          if (ior_file_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for writing: %p\n",
                               get_opts.optarg, "open"), -1);

          result = ACE_OS::fgets (temp_buf, MAX_IOR_SIZE, ior_file_);
          if (result == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to read ior from file %s: %p\n",
                               get_opts.optarg,
                               "fgets"),
                              -1);

          remote_reactor_key =
            CORBA::string_copy (temp_buf);
          ACE_OS::fclose (ior_file_);
        }
	break;
      case 'k':
        remote_reactor_key =
          CORBA::string_copy (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f ior-key-file]"
                           " [-k ior-as-string]"
                           "\n",
                           argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char *argv[])
{
  TAO_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "client",
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the object adapter.
      CORBA::Object_var poa_object =
	orb->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (poa_object.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      parse_args (argc, argv);

      if (remote_reactor_key.in () == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: no remote Reactor key specified\n"),
                          -1);

      CORBA::Object_var reactor_object =
        orb->string_to_object (ACE_const_cast(char*, remote_reactor_key.in ()),
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;

      Reactor_var remote_reactor = Reactor::_narrow (reactor_object.in (),
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (remote_reactor.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: invalid Reactor key <%s>\n",
                           argv[0], remote_reactor_key.in ()), -1);

      // Create an EventHandler servant to hand to the other side...
      auto_ptr<EventHandler_i> eh_impl (new EventHandler_i);
      EventHandler_var eh = eh_impl->_this (TAO_TRY_ENV);

      // Get into the event loop briefly...just to make sure that the
      // ORB gets a chance to set things up for us to be a server.
      // What this really means is that there's a listening port.
      //
      // The bad thing is that we have to do something non-standard
      // such as call orb->run() with a zero timeout.  It would be
      // nice if the spec gave us a standard way to do this.
      if (orb->run (ACE_Time_Value::zero) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: %p\n",
                           argv[0], "unable to get the ORB Core to listen"),
                          -1);

      // Now, after all that, we can invoke an operation on the remote
      // side.
      CORBA::Long r = remote_reactor->register_handler (eh.in (), TAO_TRY_ENV);

      // We ought to have a result!
      ACE_DEBUG ((LM_DEBUG,
                  "%s: received %d as return from register_handler ()\n",
                  argv[0], r));

      remote_reactor->set_value (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      remote_reactor->decrement (eh.in (), 5, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      remote_reactor->stop (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("main");
      return 1;
    }
  TAO_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<EventHandler_i>;
template class ACE_Auto_Basic_Ptr<EventHandler_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<EventHandler_i>
#pragma instantiate ACE_Auto_Basic_Ptr<EventHandler_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
