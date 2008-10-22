// $Id$

#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>
#include <fstream>
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    //Get reference to Root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate POA Manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Find the Naming Service
    obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root =
      CosNaming::NamingContextExt::_narrow(obj.in());
    if (CORBA::is_nil(root.in())) {
      std::cerr << "Nil Naming Context reference" << std::endl;
      return 1;
    }

    // Bind a new context.
    CosNaming::Name name;
    name.length( 1 );
    name[0].id = CORBA::string_dup( "root.esc-dot" );
    name[0].kind = CORBA::string_dup( "kind1" );

    try {
      obj = root->resolve(name);
    }
    catch(const CosNaming::NamingContext::NotFound&) {
      CosNaming::NamingContext_var dummy = root->bind_new_context(name);
    }

    name.length( 2 );
    name[1].id = CORBA::string_dup( "leaf/esc-slash" );
    name[1].kind = CORBA::string_dup( "kind2" );

    // Create an object
    Messenger_i servant;
    PortableServer::ObjectId_var oid = poa->activate_object(&servant);
    CORBA::Object_var messenger_obj = poa->id_to_reference(oid.in());
    root->rebind(name, messenger_obj.in());

    // Also try rebinding to a simple path.
    CosNaming::Name_var simp_name = root->to_name("Simple");
    try {
      obj = root->resolve(simp_name.in());
    }
    catch(const CosNaming::NamingContext::NotFound&) {
      CosNaming::NamingContext_var dummy =
        root->bind_new_context(simp_name.in());
    }
    simp_name = root->to_name("Simple/Messenger");
    root->rebind(simp_name.in(), messenger_obj.in());

    // Convert Name to String Name.
    CORBA::String_var str_name = root->to_string(name);
    std::cout << "str_name:  " << str_name.in() << std::endl;
    CORBA::String_var str_simple = root->to_string(simp_name.in());
    std::cout << "simple: " << str_simple.in() << std::endl;

    // Convert String Name to Name.
    CosNaming::Name_var tname = root->to_name(str_name.in());

    std::cout << "converted back to a CosNaming::Name: " << std::endl;
    std::cout << "   name[0] = " << (* tname)[0].id.in() << " , "
         << (* tname)[0].kind.in() << std::endl;
    std::cout << "   name[1] = " << (* tname)[1].id.in() << " , "
         << (* tname)[1].kind.in() << std::endl;

    // Find the application object by resolve_str.
    try {
      obj = root->resolve_str(str_name.in());
    }
    catch(const CosNaming::NamingContext::NotFound&) {
      std::cerr<<"Couldn't resolve the string name:  " << str_name << std::endl;
      return 1;
    }

    // Create an URL string for application object.
    CORBA::String_var address = CORBA::string_dup (":localhost:2809/key/str");

    std::cout << "call to_url(\"" << address.in() << "\"" << std::endl;
    std::cout << "           ,\"" << str_simple.in() << "\")"<< std::endl;

    CORBA::String_var url_string = root->to_url(address.in(), str_simple.in());

    std::cout << "to_url result: " << url_string.in() << std::endl;


    // Write NS url to a file to let client read NS URL to get
    // NamingContext reference.
    CORBA::String_var ns_addr = CORBA::string_dup(":localhost:2809");

    std::cout << "call to_url(\"" <<ns_addr.in() << "\",\""
         << str_simple.in() << "\")"<< std::endl;

    CORBA::String_var url = root->to_url(ns_addr.in(), str_simple.in());
    std::cout << "to_url result:    " << url.in() << std::endl;


    std::ofstream iorFile("url.ior");
    iorFile << url.in() << std::endl;
    iorFile.close();

    std::cout << "Naming Service URL written to file url.ior" << std::endl;

    // Accept requests
    orb->run();
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
