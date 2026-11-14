#include "orbsvcs/Log_Macros.h"
#include "EventChannelFactory_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>
#include <string>

struct Options
{
  std::string id;
  std::string kind;
  std::basic_string<ACE_TCHAR> output;
};

int parse_args (int argc, ACE_TCHAR * argv[], Options & options)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:k:o:"));
  int opt {};
  int result {};
  while ((opt = get_opt ()) != -1)
  {
    switch (opt)
    {
    case 'i':
      options.id = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
      break;
    case 'k':
      options.kind = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
      break;
    case 'o':
      options.output = get_opt.opt_arg ();
      break;
    default:
      result = -1;
      break;
    }
  }

  if (result != 0 || (options.id.empty () && options.output.empty ()))
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("Usage: %s\n")
      ACE_TEXT ("  [-i id]  set the id that is used to register to the naming service\n")
      ACE_TEXT ("  [-k kind] set the kind that is used to register to the naming service\n")
      ACE_TEXT ("  [-o filename] set the output file name for the IOR\n")
      ACE_TEXT ("\n"),
      argv [0]));
    return -1;
  }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    Options options {};
    if (parse_args (argc, argv, options) != 0)
      return -1;

    CORBA::Object_var obj =
      orb->resolve_initial_references ("RootPOA");

    PortableServer::POA_var poa =
      PortableServer::POA::_narrow (obj.in ());

    PortableServer::POAManager_var mgr = poa->the_POAManager ();

    mgr->activate ();

    EventChannelFactory_i servant ("factory.cfg", orb.in ());

    FT::GenericFactory_var event_channel_factory =
      servant._this ();

    // register to the Event Service

    if (!options.id.empty ())
    {
      CORBA::Object_var namng_contex_object =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (namng_contex_object.in ());

      // register to naming service
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (options.id.c_str ());
      if (!options.kind.empty ())
        name[0].kind = CORBA::string_dup (options.kind.c_str ());

      naming_context->bind (name, event_channel_factory.in ());

      ORBSVCS_DEBUG ((LM_DEBUG, "Register to naming service with %s", options.id.c_str ()));
      if (!options.kind.empty ())
        ORBSVCS_DEBUG ((LM_DEBUG, ", %s", options.kind.c_str ()));
      ORBSVCS_DEBUG ((LM_DEBUG, "\n"));
    }

    if (!options.output.empty ())
    {
      // get the IOR of factory
      CORBA::String_var str = orb->object_to_string (event_channel_factory.in ());

      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (options.output.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
      if (!output_file)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                               options.output.c_str ()),
                              1);
      ACE_OS::fprintf (output_file.get (), "%s", str.in ());
    }

    orb->run ();
  }
  catch (CORBA::Exception const & ex)
  {
    ex._tao_print_exception ("A CORBA Exception occurred.");
  }

  return 0;
}

