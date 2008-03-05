#include <orbsvcs/orbsvcs/CosNamingC.h>
#include "Mocks/ComponentServer_Impl.h"
#include <tao/ORB.h>

int main(int argc, char* argv[])
  {

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
    PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
    poa_manager->activate ();

    obj = orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming = CosNaming::NamingContext::_narrow (obj.in ());
    
    ComponentServer_Impl* comp_server = 0;
    
    ACE_NEW_THROW_EX (comp_server,
                      ComponentServer_Impl (orb.in (), poa.in ()),
                      CORBA::NO_MEMORY());
    
    PortableServer::ObjectId_var oid = poa->activate_object (comp_server);
    obj = poa->id_to_reference (oid.in ());
    CdmwCcmComponentServer::ComponentServer_var server = CdmwCcmComponentServer::ComponentServer::_narrow (obj.in ());
    
    CosNaming::Name name(1);
    name.length(1);
    name[0].id = CORBA::string_dup("MockTest");
    name[0].kind = CORBA::string_dup("ComponentServer");
    naming->bind (name, server.in ());
    
    orb->run ();
    return 0;
  }
