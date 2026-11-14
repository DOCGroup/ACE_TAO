/// @author Iliyan Jeliazkov <iliyan@ociweb.com>
/// @author Lothar Werzinger <lothar@tradescape.biz>

#include "ace/Service_Config.h"
#include "tao/corba.h"
#include "ace/OS_NS_unistd.h"

constexpr ACE_TCHAR const *scpc_loadOrb = // NOTE due to the way ACE_DYNAMIC_SERVICE_DIRECTIVE() macro is defined,
  // each parameter CANNOT be split into multiple quoted strings "line1" "nextline" with the expectation that
  // they will be join together. Hence the long parameter 4.
  ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE ("testDllOrb",
                                 "DllOrb",
                                 TAO_VERSION,
                                 "_make_DllOrb",
"DllOrb -t 1 -ORBGestalt Local -ORBDebugLevel 3 -ORBId testORB \
  -ORBInitRef NameService=file:///tmp/test-ns.ior \
  -ORBDottedDecimalAddresses 1"
                                 );

constexpr ACE_TCHAR const *scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE ("testDllOrb");

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nbefore first load process directive\n")));
  if (ACE_Service_Config::process_directive (scpc_loadOrb) < 0)
    return -1;

  ACE_OS::sleep (2);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nbefore first unload process directive\n")));
  if (ACE_Service_Config::process_directive (scpc_unloadOrb) < 0)
    return -1;

  ACE_OS::sleep (2);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nMAIN (%P|%t) Lather, Rinse, Repeat ...\n")));

  if (ACE_Service_Config::process_directive (scpc_loadOrb) < 0)
    return -1;

  ACE_OS::sleep (2);

  if (ACE_Service_Config::process_directive (scpc_unloadOrb) < 0)
    return -1;

  return 0;
}
