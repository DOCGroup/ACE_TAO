/*
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 * @author Lothar Werzinger <lothar@tradescape.biz>
 */

#include "ace/OS.h"
#include "ace/Service_Config.h"
#include "tao/corba.h"

char const * const scpc_loadOrb =
  ACE_DYNAMIC_SERVICE_DIRECTIVE ("testDllOrb",
                                 "DllOrb",
                                 "_make_DllOrb",
                                 "DllOrb -ORBDebugLevel 30 -ORBId testORB -ORBInitRef "
                                 "NameService=file:///tmp/test-ns.ior -ORBDottedDecimalAddresses 1"
                                 );

char const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE ("testDllOrb");

int main(int, char **)
{
  if (0 > ACE_Service_Config::process_directive(scpc_loadOrb))
    return -1;

  ACE_OS::sleep(2);

  if (0 > ACE_Service_Config::process_directive(scpc_unloadOrb))
    return -1;

  ACE_OS::sleep(2);
  ACE_DEBUG ((LM_DEBUG, "MAIN (%P|%t) Lather, Rince, Repeat ...\n"));

  if (0 > ACE_Service_Config::process_directive(scpc_loadOrb))
    return -1;

  ACE_OS::sleep(2);

  if (0 > ACE_Service_Config::process_directive(scpc_unloadOrb))
    return -1;

  return 0;
}
