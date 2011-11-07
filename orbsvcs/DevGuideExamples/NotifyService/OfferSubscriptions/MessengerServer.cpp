// $Id$

#include "orbsvcs/CosNamingC.h"
#include "Messenger_i.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
      ;
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try
   {
     CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

   CORBA::Object_var rootObj = orb->resolve_initial_references("NameService");

     CosNaming::NamingContext_var rootNC =
       CosNaming::NamingContext::_narrow(rootObj.in());

     // Get reference to Root POA.
     CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");

     PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

     // Activate POA manager
     PortableServer::POAManager_var mgr = poa->the_POAManager();

     mgr->activate();

     // Create an object
     PortableServer::Servant_var<Messenger_i> messenger_servant =
       new Messenger_i(orb.in());

     Messenger_var messenger = messenger_servant->_this();

     CosNaming::Name name;
     name.length (1);
     name[0].id = CORBA::string_dup("MessengerService");

     rootNC->rebind(name, messenger.in());

     CORBA::String_var str = orb->object_to_string (messenger.in());
     std::ofstream iorFile (ACE_TEXT_ALWAYS_CHAR(ior_output_file));
     iorFile << str.in () << std::endl;
     iorFile.close ();
     std::cout << "IOR written to file " << ior_output_file << std::endl;

     // Accept requests
     orb->run();
   }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "MessengerServer::Caught exception: " << ex << std::endl;
    return 1;
  }

 return 0;
}









