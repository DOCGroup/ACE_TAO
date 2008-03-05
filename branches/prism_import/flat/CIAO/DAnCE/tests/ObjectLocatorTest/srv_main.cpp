
#include "ace/OS_NS_stdio.h"

#include "DummyC.h"
#include "DummyImpl.h"
#include "RedirectionService/CCMObjectLocator.h"
#include "RegistrationPath.h"

using namespace ObjectLocatorTest;


int main (int argc, char * argv[])
{
  try
    {
      int argcm = argc + 2;
      char * argvm[argcm+1];
      for (int i = 0; i < argc; ++i)
        {
          argvm[i] = argv[i];
        }
      argvm[argcm-2] = "-ORBListenEndpoints";
      char buf[32];
      ACE_OS::sprintf (buf, "iiop://:12345");
      argvm[argcm-1] = buf;
      argvm[argcm] = 0;

      CORBA::ORB_var orb = CORBA::ORB_init (argcm, argvm);

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager();
      poa_manager->activate();

      DAnCE::CCMObjectLocator locator (orb.in(), root_poa.in(), "Dummy");

      DummyImpl servant;

      PortableServer::ObjectId_var id = root_poa->activate_object (&servant);
      Dummy_var dummy_obj = Dummy::_narrow (root_poa->id_to_reference (id));//servant._this();

      locator.register_object (app_name, inst_name, port_name, dummy_obj);

      orb->run();
    }
  catch (...)
    {
      ACE_ERROR((LM_ERROR, "[%M] An error has occured."));
    }
}

