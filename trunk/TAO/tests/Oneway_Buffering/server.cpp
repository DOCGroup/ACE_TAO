// $Id$

#include "Oneway_Buffering.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID(Oneway_Buffering, server, "$Id$")

const ACE_TCHAR *ior_output_file = ACE_TEXT("server.ior");
const ACE_TCHAR *ior = ACE_TEXT("file://admin.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Oneway_Buffering_Admin_var admin =
        Test::Oneway_Buffering_Admin::_narrow(tmp.in ());

      if (CORBA::is_nil (admin.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Oneway_Buffering_Admin reference <%s>\n",
                             ior),
                            1);
        }

      Oneway_Buffering *oneway_buffering_impl;
      ACE_NEW_RETURN (oneway_buffering_impl,
                      Oneway_Buffering (orb.in (),
                                        admin.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(oneway_buffering_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (oneway_buffering_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Oneway_Buffering_var oneway_buffering =
        Test::Oneway_Buffering::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (oneway_buffering.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in server:");
      return 1;
    }

  return 0;
}
