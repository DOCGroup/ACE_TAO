// $Id$
// This is a simple example, showing how you can load an
// ImR Locator and/or Activator as ACE Service Objects
// using the ACE Service Configurator framework.

#include "service_configS.h"

#include "ace/Service_Config.h"
#include "ace/streams.h"
#include "ace/OS_NS_string.h"


using namespace CORBA;
using namespace PortableServer;

class SvcConf
  : public POA_ServiceConfigurator
{
  ACE_Service_Config& asc_;
public:
  SvcConf(ACE_Service_Config& asc)
    : asc_(asc)
  {
  }
  virtual CORBA::Long process_directive(const char* s)
  {
    ACE_ASSERT(s != 0);
    ACE_ASSERT(ACE_OS::strlen(s) > 0);
    return asc_.process_directive(s);
  }

  virtual void reconfigure()
  {
    asc_.reconfigure();
  }
};

int main (int argc, char* argv[])
{
  try {

    ACE_Service_Config config;
    config.open(argc, argv);

    ORB_var orb = ORB_init(argc, argv);

    Object_var obj = orb->resolve_initial_references("RootPOA");
    POA_var poa = POA::_narrow(obj.in());
    ACE_ASSERT(! is_nil(poa.in()));
    POAManager_var poaman = poa->the_POAManager();

    SvcConf svt(config);

    ObjectId_var id = poa->activate_object(&svt);
    obj = poa->id_to_reference(id.in());
    ACE_ASSERT(! is_nil(obj.in()));
    String_var ior = orb->object_to_string(obj.in());

    poaman->activate();

    {
      ofstream out("combined.ior");
      out << ior;
    }

    ACE_DEBUG((LM_DEBUG, "Combined service started.\n"));

    orb->run();

    ACE_DEBUG((LM_DEBUG, "Combined service shutdown.\n"));

    poa->destroy(1, 1);
    orb->destroy();

  } catch (CORBA::Exception& e) {
    e._tao_print_exception ("Combined Service:");
  }
  return 0;
}

