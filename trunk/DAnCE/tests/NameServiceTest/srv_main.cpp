// $Id$
#include "ace/OS_NS_stdio.h"
#include <orbsvcs/CosNamingC.h>
#include <orbsvcs/Naming/Naming_Loader.h>
#include "Logger/Log_Macros.h"

#include "DummyC.h"
#include "DummyImpl.h"
#include "RegistrationPath.h"

using namespace NameServiceTest;

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      int argcm = argc + 2;
      ACE_TCHAR ** argvm = new ACE_TCHAR* [argcm+1];
      for (int i = 0; i < argc; ++i)
        {
          argvm[i] = argv[i];
        }
      ACE_TCHAR buf1[32];
      ACE_OS::sprintf (buf1, ACE_TEXT("-ORBListenEndpoints"));
      argvm[argcm-2] = buf1;
      ACE_TCHAR buf2[32];
      ACE_OS::sprintf (buf2, ACE_TEXT("iiop://:12345"));
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

      CosNaming::Name name (1);

      name.length (1);

      name[0].id = CORBA::string_dup (app_name);
      name[0].kind = CORBA::string_dup ("DeploymentPlan");

      CosNaming::NamingContext_var naming_app = naming->bind_new_context (name);

      name[0].id = CORBA::string_dup ("Dummy");
      name[0].kind = CORBA::string_dup ("");
      naming_app->bind (name, dummy_obj.in());

      orb->run();
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "An error has occured."));
    }
  return 0;
}

