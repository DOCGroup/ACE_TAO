// -*- C++ -*-

//=============================================================================
/**
 *  @file    server.cpp
 *
 *  C++ server side for testing interoperability with wchar data.
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#include "interop_wchar_i.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include <memory>
#if defined (TAO_EXPLICIT_NEGOTIATE_CODESETS)
#include "tao/Codeset/Codeset.h"
#endif /* TAO_EXPLICIT_NEGOTIATE_CODESETS */

ACE_TCHAR const *ior_output_file = ACE_TEXT ("IOR");
bool verbose {};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:v"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'v':
        verbose = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-v "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter const command_line (argc, argv);

#if (!defined ACE_HAS_WCHAR) && (!defined ACE_HAS_XPG4_MULTIBYTE_CHAR)
  // the run_test script looks for the ior file. By touching it here, the
  // script can run at full speed, rather than timing out waiting for a
  // file that will never come.
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("Cannot open output file ")
                       ACE_TEXT("for writing IOR: %s"),
                       ior_output_file),
                      1);
  ACE_OS::fprintf (output_file.get (), "no ior\n");
  ACE_ERROR_RETURN ((LM_ERROR,"This test requires wchar support\n"),0);
#else
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("failed to parse args")),
                          1);

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();
      mgr->activate ();

      interop_WChar_Passer_i servant (orb.in (), verbose);
      obj = poa->servant_to_reference (&servant);
      CORBA::String_var const str = orb->object_to_string (obj.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("Cannot open output file ")
                             ACE_TEXT("for writing IOR: %s"),
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", str.in ());
      }

      orb->run ();
      orb->destroy ();
    }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("uncaught exception");
        return 1;
      }

  return 0;
#endif /* ACE_HAS_WCHAR */
}
