#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
/// include this file to statically linked with FT ORB
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

/// include this file to statically linked with Transaction Depth
#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h"

#include <memory>
#include <string>

struct Options
{
  std::string name { "FT_EventService" };
  std::basic_string<ACE_TCHAR> ior_file_name;
};

int parse_args (
  int argc,
  ACE_TCHAR * argv[],
  CORBA::ORB_ptr orb,
  Options & options,
  FtRtecEventChannelAdmin::EventChannel_var & ftec)
{
  try
  {
    ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:n:o:"));
    int opt {};
    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup ("FT_EventService");

    while ((opt = get_opt ()) != -1)
    {
      switch (opt)
      {
      case 'i':
        {
          CORBA::Object_var const obj = orb->string_to_object (get_opt.opt_arg ());
          ftec = FtRtecEventChannelAdmin::EventChannel::_narrow (obj.in ());
        }
        break;
      case 'n':
        options.name = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
        break;
      case 'o':
        options.ior_file_name = get_opt.opt_arg ();
        break;
      }
    }

    if (CORBA::is_nil (ftec.in ()))
    {
      /// we should get the ftec from Naming Service
      name[0].id = CORBA::string_dup (options.name.c_str ());

      CosNaming::NamingContext_var naming_context =
        resolve_init<CosNaming::NamingContext> (orb, "NameService");

      ftec = resolve<FtRtecEventChannelAdmin::EventChannel> (
        naming_context.in (), name);

      if (CORBA::is_nil (ftec.in ()))
        ORBSVCS_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot Find FT_EventService\n")), -1);
    }
  }
  catch (CORBA::Exception const &)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot Find FT_EventService\n")), -1);
  }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    FtRtecEventChannelAdmin::EventChannel_var ftec;
    Options options {};

    if (parse_args (argc, argv, orb.in (), options, ftec) != 0)
      return 1;

    PortableServer::POA_var
      root_poa = resolve_init<PortableServer::POA> (orb.in (), "RootPOA");

    // create POAManager
    PortableServer::POAManager_var
      mgr = root_poa->the_POAManager ();
    mgr->activate ();

    TAO_FTRTEC::FTEC_Gateway gateway_servant(orb.in(), ftec.in());

    RtecEventChannelAdmin::EventChannel_var gateway =
      gateway_servant.activate (root_poa.in ());

    if (!options.ior_file_name.empty ())
    {
      CORBA::String_var const str = orb->object_to_string (gateway.in ());

      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (options.ior_file_name.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
      if (!output_file)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
          ACE_TEXT ("Cannot open output file for writing IOR: %s"),
          options.ior_file_name.c_str ()),
          1);
      ACE_OS::fprintf (output_file.get (), "%s", str.in ());
    }

    orb->run ();
  }
  catch (CORBA::Exception const &)
  {
    return 1;
  }
  return 0;
}
