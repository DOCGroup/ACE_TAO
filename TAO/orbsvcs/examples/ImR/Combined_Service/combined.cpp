// This is a simple example, showing how you can load an
// ImR Locator and/or Activator as ACE Service Objects
// using the ACE Service Configurator framework.

#include "service_configS.h"

#include <cstring>
#include <fstream>

#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

using namespace CORBA;
using namespace PortableServer;

class SvcConf : public POA_ServiceConfigurator
{
  ACE_Service_Config &asc_;
public:
  explicit SvcConf (ACE_Service_Config &asc)
    : asc_ (asc)
  {
  }

  SvcConf (SvcConf const &) = delete;
  SvcConf &operator= (SvcConf const &) = delete;
  SvcConf (SvcConf &&) = delete;
  SvcConf &operator= (SvcConf &&) = delete;

  CORBA::Long process_directive (char const *s) override
  {
    ACE_ASSERT (s != nullptr);
    ACE_ASSERT (std::strlen (s) > 0);
    return asc_.process_directive (ACE_TEXT_CHAR_TO_TCHAR (s));
  }

  void reconfigure () override
  {
    asc_.reconfigure ();
  }
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    ACE_Service_Config config;
    config.open (argc, argv);

    ORB_var orb = ORB_init (argc, argv);

    Object_var obj = orb->resolve_initial_references ("RootPOA");
    POA_var poa = POA::_narrow (obj.in ());
    ACE_ASSERT (!is_nil (poa.in ()));
    POAManager_var poaman = poa->the_POAManager ();

    SvcConf svt (config);

    ObjectId_var id = poa->activate_object (&svt);
    obj = poa->id_to_reference (id.in ());
    ACE_ASSERT (!is_nil (obj.in ()));
    String_var ior = orb->object_to_string (obj.in ());

    poaman->activate ();

    {
      std::ofstream out ("combined.ior");
      out << ior.in ();
    }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Combined service started.\n")));

    orb->run ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Combined service shutdown.\n")));

    poa->destroy (true, true);
    orb->destroy ();
  } catch (CORBA::Exception const &e) {
    e._tao_print_exception ("Combined Service:");
    return 1;
  }
  return 0;
}
