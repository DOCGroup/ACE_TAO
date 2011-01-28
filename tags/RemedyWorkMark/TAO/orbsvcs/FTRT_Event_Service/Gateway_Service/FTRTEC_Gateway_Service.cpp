// $Id$

#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
/// include this file to statically linked with FT ORB
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

/// include this file to statically linked with Transaction Depth
#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h"

namespace {
  CORBA::ORB_var orb;
  FtRtecEventChannelAdmin::EventChannel_var ftec;
  ACE_TString ior_file_name;
}

int parse_args (int argc, ACE_TCHAR *argv[])
{
  try{
    ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:n:o:"));
    int opt;
    CosNaming::Name name(1);
    name.length(1);
    name[0].id = CORBA::string_dup ("FT_EventService");

    while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
      {
      case 'i':
        {
          CORBA::Object_var obj = orb->string_to_object (get_opt.opt_arg ());
          ftec = FtRtecEventChannelAdmin::EventChannel::_narrow (obj.in());
        }
        break;
      case 'n':
        name[0].id = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ()));
        break;
      case 'o':
        ior_file_name = get_opt.opt_arg ();
        break;
      }
    }

    if (CORBA::is_nil(ftec.in())) {
      /// we should get the ftec from Naming Service

      CosNaming::NamingContext_var naming_context =
        resolve_init<CosNaming::NamingContext>(orb.in(), "NameService");

      ftec = resolve<FtRtecEventChannelAdmin::EventChannel>(naming_context.in(),
        name);

      if (CORBA::is_nil(ftec.in()))
        ACE_ERROR_RETURN((LM_ERROR, "Cannot Find FT_EventService\n"), -1);
    }
  }
  catch (const CORBA::Exception&){
    ACE_ERROR_RETURN((LM_ERROR, "Cannot Find FT_EventService\n"), -1);
  }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) == -1)
      return 1;

    PortableServer::POA_var
      root_poa =  resolve_init<PortableServer::POA>(orb.in(), "RootPOA");

    // create POAManager
    PortableServer::POAManager_var
      mgr = root_poa->the_POAManager();


    mgr->activate();

    TAO_FTRTEC::FTEC_Gateway gateway_servant(orb.in(), ftec.in());

    RtecEventChannelAdmin::EventChannel_var gateway =
      gateway_servant.activate(root_poa.in());

    if (ior_file_name.length())
    {
      CORBA::String_var str = orb->object_to_string(gateway.in());

      FILE *output_file=
        ACE_OS::fopen (ior_file_name.c_str(), "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
        "Cannot open output file for writing IOR: %s",
        ior_file_name.c_str()),
        1);
      ACE_OS::fprintf (output_file, "%s", str.in ());
      ACE_OS::fclose (output_file);
    }

    orb->run();
  }
  catch (const CORBA::Exception&){
    return 1;
  }
  return 0;
}
