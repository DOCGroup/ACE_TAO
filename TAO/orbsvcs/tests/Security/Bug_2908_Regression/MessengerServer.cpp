#include "Messenger_i.h"
#include "ace/Get_Opt.h"
#include <orbsvcs/SecurityLevel2C.h>
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -o <iorfile>\n"),
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
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    Server_Options options;

    if (parse_args (argc, argv, options) != 0)
      return 1;

    //Get reference to Root POA
    CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj.in ());

    // Activate POA Manager
    PortableServer::POAManager_var mgr = poa->the_POAManager ();
    mgr->activate ();

    // Create an object
    Messenger_i messenger_servant (orb);
    Messenger_var messenger_factory = messenger_servant._this ();

    // In order to allow collocated invocations we need to allow unsecured
    // collocated invocations to the object else our security manager will
    // block the collocated invocation unless you explicitly allow it
    CORBA::Object_var sec_man =
      orb->resolve_initial_references ("SecurityLevel2:SecurityManager");
    SecurityLevel2::SecurityManager_var sec2manager =
      SecurityLevel2::SecurityManager::_narrow (sec_man.in ());
    SecurityLevel2::AccessDecision_var ad_tmp =
      sec2manager->access_decision ();
    TAO::SL2::AccessDecision_var ad =
      TAO::SL2::AccessDecision::_narrow (ad_tmp.in ());
    // Allow unsecured collocated invocations
    ad->default_collocated_decision (true);

    CORBA::String_var ior =
      orb->object_to_string (messenger_factory.in ());

    // If the ior_output_file exists, output the ior to it
    if (!options.ior_output_file.empty ())
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

    // Accept requests
    orb->run ();

    poa->destroy (true, true);

    orb->destroy ();
  }
  catch (CORBA::Exception const &)
  {
    ACE_ERROR ((LM_ERROR, "Caught a CORBA exception: "));
    return 1;
  }

  return 0;
}
