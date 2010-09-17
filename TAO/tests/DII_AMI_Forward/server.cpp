// -*- C++ -*-

//
// $Id$
//

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "server_interceptor.h"
#include "orb_initializer.h"
#include "tao/ORBInitializer_Registry.h"

const ACE_TCHAR *ior_filename = ACE_TEXT("server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:m:n:t"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'o':
          ior_filename = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s "
                             "-o <iorfile>"
                             "\n",
                             argv [0]),
                            -1);
      }
    // Indicates successful parsing of the command line
    return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Create the ORB initializer.
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);

      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      // Now initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      // Create the interceptor.
      ForwardTest_Request_Interceptor * server_interceptor =
        temp_initializer->server_interceptor();

      Forward_Test_i *final_impl = new Forward_Test_i(orb.in ());
      PortableServer::ServantBase_var servant = final_impl;
      Forward::Test_var target = final_impl->_this ();

      server_interceptor->forward_reference (target.in());

      Forward_Test_i *server_impl = new Forward_Test_i (orb.in());
      servant = server_impl;
      Forward::Test_var server = server_impl->_this ();

      CORBA::String_var iorstr =
        orb->object_to_string (server.in ());

      FILE *output_file= ACE_OS::fopen (ior_filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_filename),
                          1);
      ACE_OS::fprintf (output_file, "%s", iorstr.in());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server: exception caught - ");
      return 1;
    }

  return 0;
}
