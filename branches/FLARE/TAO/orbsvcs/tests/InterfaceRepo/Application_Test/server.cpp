// -*- C++ -*-
// $Id$

#include "test_i.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID(Application_Test, server, "$Id$")

static const char *ior_output_file = "iorfile";

static int
write_ior_to_file (const char *ior)
{
  FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

  if (output_file == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output files for writing IOR: %s\n",
                         ior_output_file),
                        -1);
    }

  int result = ACE_OS::fprintf (output_file,
                                "%s",
                                ior);
  if (result < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_OS::fprintf failed while writing %s to %s\n",
                         ior,
                         ior_output_file),
                        -1);
    }

  ACE_OS::fclose (output_file);

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
        try
          {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      inventory_i servant_impl;

      warehouse::inventory_var servant =
        servant_impl._this ();

      CORBA::String_var ior =
        orb->object_to_string (servant.in ());

      int write_result = write_ior_to_file (ior.in ());

      if (write_result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p\n",
                             "write_ior_to_file"),
                            -1);
        }

      orb->run ();

      root_poa->destroy (1,
                         1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server exception:");
      return 1;
    }

  return 0;
}
