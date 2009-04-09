// $Id$
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include <orbsvcs/orbsvcs/Naming/Naming_Loader.h>
#include "DAnCE/Logger/Log_Macros.h"
#include "RedirectionService/RedirectionService.h"

#include "DummyC.h"
#include "DummyImpl.h"
#include "RegistrationPath.h"

using namespace Test;

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
//    try
  {
    int argcm = argc + 2;
    ACE_TCHAR ** argvm = new ACE_TCHAR* [argcm+1];
    for (int i = 0; i < argc; ++i)
      {
        argvm[i] = argv[i];
      }
    char buf1[32];
    ACE_OS::sprintf (buf1, "-ORBListenEndpoints");
    argvm[argcm-2] = buf1;
    char buf2[32];
    ACE_OS::sprintf (buf2, "iiop://:12345");
    argvm[argcm-1] = buf2;
    argvm[argcm] = 0;

    CORBA::ORB_var orb = CORBA::ORB_init (argcm, argvm);

    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager();
    poa_manager->activate();

    DummyImpl servant;

    PortableServer::ObjectId_var id = root_poa->activate_object (&servant);

    Dummy_var dummy_obj = Dummy::_narrow (root_poa->id_to_reference (id));//servant._this();

    TAO_Naming_Loader loader;

    CORBA::Object_var obj_tmp = loader.create_object (orb.in(), argcm, argvm);
    CORBA::Object_var naming_obj = orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var naming = CosNaming::NamingContext::_narrow (naming_obj.in());

    DAnCE::RedirectionService redirection (orb.in(), root_poa.in(), naming.in(), CosNaming::NamingContext::_nil(), true, true);

    redirection.registration_start (node_name, app_name);
    redirection.registration (node_name, app_name, inst_name, port_name, dummy_obj.in());
    DANCE_DEBUG((LM_DEBUG, "[%M] Registration for port have finished.\n\n"));
    redirection.registration (node_name, app_name, inst_name, /*CORBA::Object::_nil()*/dummy_obj.in());
    DANCE_DEBUG((LM_DEBUG, "[%M] Registration for component have finished.\n\n"));
    redirection.registration_finish (node_name, app_name);
    orb->run();
  }
//    catch (...)
//    {
//        DANCE_ERROR((LM_ERROR, "[%M] An error has occured."));
//    }
  return 0;
}

