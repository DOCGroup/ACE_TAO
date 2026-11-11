// file      : RolyPoly/server.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

#include "RolyPoly_i.h"
#include "CrashPoint.h"
#include "ORB_Initializer.h"
#include "tao/ORBInitializer_Registry.h"

struct Server_Options
{
  ACE_TCHAR const *ior_file {};
  short crash_point {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:c:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_file = get_opts.opt_arg ();
        break;
      case 'c':
        options.crash_point = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage:  %s -o <IOR> -c <CrashPoint>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options {};
      if (::parse_args (argc, argv, options) != 0)
        return -1;
      crash_point = options.crash_point;

      ORB_Initializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      ORB_Initializer,
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
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      RolyPoly_i *roly_poly_impl {};

      ACE_NEW_RETURN (roly_poly_impl,
                      RolyPoly_i (orb.in ()),
                      1);

      PortableServer::ServantBase_var const owner_transfer (roly_poly_impl);

      RolyPoly_var const t =
        roly_poly_impl->_this ();

      CORBA::String_var const ior =
        orb->object_to_string (t.in ());

      poa_manager->activate ();

      std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
        ACE_OS::fopen (options.ior_file, ACE_TEXT ("w")), ACE_OS::fclose);
      if (!output_file)
        {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file <%s> for writing IOR: %C"),
                           options.ior_file,
                           ior.in ()),
                          1);
        }

      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server is ready\n")));

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Event loop finished.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Caught exception:"));
      return -1;
    }

  return 0;
}
