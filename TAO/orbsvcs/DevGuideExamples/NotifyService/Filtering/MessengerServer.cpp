// $Id$

#include "orbsvcs/CosNamingC.h"
#include "Messenger_i.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Get_Opt.h"
#include <iostream>
#include <fstream>

ACE_TString ior_output_file;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'o':
          ior_output_file = get_opts.optarg;
          break;

        case '?':
        default:
          std::cerr << "usage: " << argv[0] << " -o <ior>" << std::endl;
          return -1;
          break;
      }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  try
     {
       // Initialize orb
       ACE_Argv_Type_Converter conv(argc, argv);
       CORBA::ORB_var orb = CORBA::ORB_init(conv.get_argc(),
                                            conv.get_TCHAR_argv());

       if (parse_args(argc, argv) != 0) {
         return 1;
       }

       // Find the Naming Service.
       CORBA::Object_var rootObj =  orb->resolve_initial_references("NameService");
       CosNaming::NamingContext_var rootNC =
          CosNaming::NamingContext::_narrow(rootObj.in());

       // Get the  Root POA.
       CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
       PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

       // Activate POA manager
       PortableServer::POAManager_var mgr = poa->the_POAManager();
       mgr->activate();

       // Create our Messenger servant.
       Messenger_i messenger_servant(orb.in());

       // Register it with the RootPOA.
       PortableServer::ObjectId_var oid =
         poa->activate_object( &messenger_servant );
       CORBA::Object_var messenger_obj = poa->id_to_reference( oid.in() );

       // Generating the ior file is just for the purpose of synchronize the
       // the startup of the server and consumer.
       CORBA::String_var ior = orb->object_to_string(messenger_obj.in());
       if (ior_output_file != ACE_TEXT("")) {
         std::ofstream outfile(ACE_TEXT_ALWAYS_CHAR(ior_output_file.c_str()));
         outfile << ior.in();
       }

       // Bind it in the Naming Service.
       CosNaming::Name name;
       name.length (1);
       name[0].id = CORBA::string_dup("MessengerService");
       rootNC->rebind(name, messenger_obj.in());

       // Accept requests
       orb->run();
       orb->destroy();

     }
  catch(const CORBA::Exception& ex)
    {
      std::cerr << ex << std::endl;
      return 1;
    }
  return 0;
}
