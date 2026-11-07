#include "GoodDay.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include <cstdio>
#include <memory>

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const*& ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage: %s [-o <iorfile>]\n"),
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_TCHAR const* ior_output_file {ACE_TEXT("test.ior")};
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P|%t) Panic: nil RootPOA\n")),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      GoodDay *goodday_impl {};
      ACE_NEW_RETURN (goodday_impl, GoodDay (orb.in ()), 1);
      PortableServer::ServantBase_var const owner_transfer (goodday_impl);

      Test::GoodDay_var const goodday = goodday_impl->_this ();

      CORBA::String_var const ior = orb->object_to_string (goodday.in ());

      // Output the IOR to the <ior_output_file>
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (ior_output_file, "w"), std::fclose);
      if (output_file == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("Cannot open output file for writing IOR: %s\n"),
                           ior_output_file),
                          1);
      std::fprintf (output_file.get (), "%s", ior.in ());

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) server - event loop finished\n")));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
