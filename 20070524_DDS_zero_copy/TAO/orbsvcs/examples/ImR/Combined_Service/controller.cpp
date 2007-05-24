// $Id$
// This is a simple test of an ImR using the corba interfaces

#include "service_configC.h"

#include <ace/streams.h>
#include <ace/SString.h>
#include <ace/Log_Msg.h>

using namespace CORBA;

namespace {

  ACE_CString directive;

  void showusage() {
    ACE_DEBUG((LM_DEBUG, "Usage: controller [-r | -c \"service_configurator_directive\"]\n"
      "(e.g. dynamic mysvc Service_Object * MySvc:_make_MySvc() \"-arg1 one\"\n\n"));
  }

  bool parse_args(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
      ACE_ERROR((LM_ERROR, "Controller: wrong number of arguments. %d\n", argc - 1));
      return false;
    }
    for (int i = 1; i < argc; ++i) {
      ACE_CString s(argv[i]);
      if (s == "-h" || s == "-help" || s == "--help" || s == "-?")
        return false;
      if (s == "-r") {
        return true;
      }
      if (s == "-c" && ++i < argc) {
        directive = argv[i];
        return true;
      }
    }
    return false;
  }
}

int main(int argc, char* argv[]) {

  try {

    ORB_var orb = ORB_init(argc, argv);

    if (! parse_args(argc, argv)) {
      showusage();
      return 1;
    }

    ACE_DEBUG((LM_DEBUG, "Controller: Connecting to combined service...\n"));

    Object_var obj = orb->resolve_initial_references("ServiceConfig");
    ServiceConfigurator_var sc = ServiceConfigurator::_narrow(obj.in());
    ACE_ASSERT(! is_nil(sc.in()));

    if (directive.length() > 0) {
      ACE_DEBUG((LM_DEBUG, "Controller: sending directive <%s>...\n", directive.c_str()));
      sc->process_directive(directive.c_str());
    } else {
      ACE_DEBUG((LM_DEBUG, "Controller: reloading config file...\n"));
      sc->reconfigure();
    }
    ACE_DEBUG((LM_DEBUG, "Controller: done.\n"));

    return 0;
  } catch (CORBA::Exception& e) {
    e._tao_print_exception ("Controller:");
  }
  return 1;
}
