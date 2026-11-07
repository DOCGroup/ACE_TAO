// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "test_i.h"
#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include <memory>

struct Options
{
  ACE_TString ior_file {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  if (argc != 3)  // foo -o IOR
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':

        options.ior_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o IOR\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  try
    {
      Server_ORBInitializer *temp_initializer {};
      // The ORB initializer var assumes ownership of the ACE_NEW allocation.
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var const orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

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

      if (::parse_args (argc, argv, options) != 0)
        return -1;

      test_i servant (orb.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&servant);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      CORBA::Object_var const obj = CORBA::Object::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Servant:\n<%C>\n",
                  ior.in ()));

      poa_manager->activate ();

      // Write IOR to a file.
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file <%s> for writing IOR: %C"),
                             options.ior_file.c_str (),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
