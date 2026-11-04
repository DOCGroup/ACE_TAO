#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_stdio.h"
#include "server_interceptor.h"
#include "orb_initializer.h"
#include "tao/ORBInitializer_Registry.h"

#include <memory>

struct Server_Options
{
  ACE_TString ior_filename {ACE_TEXT ("server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_filename = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           argv[0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  try
    {
      // The ORB registry retains the initializer; this raw pointer is borrowed
      // to access the interceptor created during ORB initialization.
      Server_ORBInitializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer, Server_ORBInitializer, -1);
      PortableInterceptor::ORBInitializer_var const initializer = temp_initializer;
      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      ForwardTest_Request_Interceptor * const server_interceptor =
        temp_initializer->server_interceptor ();

      Forward_Test_i *final_impl {};
      ACE_NEW_THROW_EX (final_impl, Forward_Test_i (orb.in ()), CORBA::NO_MEMORY ());
      // The servant wrapper owns this pointer; final_impl is used as a borrowed alias.
      PortableServer::ServantBase_var servant = final_impl;
      Forward::Test_var const target = final_impl->_this ();
      server_interceptor->forward_reference (target.in ());

      Forward_Test_i *server_impl {};
      ACE_NEW_THROW_EX (server_impl, Forward_Test_i (orb.in ()), CORBA::NO_MEMORY ());
      // The servant wrapper owns this pointer; server_impl is used as a borrowed alias.
      servant = server_impl;
      Forward::Test_var const server = server_impl->_this ();
      CORBA::String_var const iorstr = orb->object_to_string (server.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_filename.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             options.ior_filename.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", iorstr.in ());
      }

      poa_manager->activate ();
      orb->run ();
      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Server: exception caught - ");
      return 1;
    }

  return 0;
}
