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
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (::parse_args (argc, argv) != 0) return -1;

      ORB_Initializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      ORB_Initializer,
                      -1);  // No exceptions yet!

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RolyPoly_i* roly_poly_impl;

      ACE_NEW_RETURN (roly_poly_impl,
                      RolyPoly_i (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer (roly_poly_impl);

      RolyPoly_var t =
        roly_poly_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies;  // Empty policy list.

      CORBA::String_var ior =
        orb->object_to_string (t.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
