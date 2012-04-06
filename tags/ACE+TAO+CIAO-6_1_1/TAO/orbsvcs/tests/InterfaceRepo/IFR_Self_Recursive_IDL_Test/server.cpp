// $Id$

#include "GoodDay.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/IFR_Client/IFR_BasicC.h"


const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
int object_to_output = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage:  %s ")
                           ACE_TEXT("-o <iorfile>")
                           ACE_TEXT("\n"),
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT(" (%P|%t) Panic: nil RootPOA\n")),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      GoodDay *goodday_impl;
      ACE_NEW_RETURN (goodday_impl,
                      GoodDay (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(goodday_impl);

      Test::GoodDay_var goodday =
        goodday_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (goodday.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("Cannot open output file for writing IOR: %s\n"),
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());


      ACE_OS::fclose (output_file);

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) server - event loop finished\n")));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
