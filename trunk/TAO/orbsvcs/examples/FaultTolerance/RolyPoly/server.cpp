// file      : RolyPoly/server.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include "RolyPoly_i.h"
#include "CrashPoint.h"
#include "ORB_Initializer.h"
#include "tao/ORBInitializer_Registry.h"

const char *ior_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      case 'c':
        crash_point = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o <IOR> "
                           "-c <CrashPoint>\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (::parse_args (argc, argv) != 0) return -1;

      ORB_Initializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      ORB_Initializer,
                      -1);  // No exceptions yet!

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      RolyPoly_i* roly_poly_impl;

      ACE_NEW_RETURN (roly_poly_impl,
                      RolyPoly_i (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer (roly_poly_impl);

      RolyPoly_var t =
        roly_poly_impl->_this ();

      CORBA::PolicyList policies;  // Empty policy list.

      CORBA::String_var ior =
        orb->object_to_string (t.in ());

      poa_manager->activate ();

      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file <%s> for writing "
                           "IOR: %s",
                           ior.in ()),
                          1);
      }

      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      ACE_DEBUG ((LM_DEBUG, "Server is ready\n"));

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (1, 1);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
