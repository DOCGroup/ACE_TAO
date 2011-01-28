/*
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 * @author Lothar Werzinger <lothar@tradescape.biz>
 *
 * $Id$
 */

#include "ace/Service_Config.h"
#include "tao/corba.h"
#include "ace/OS_NS_unistd.h"

ACE_TCHAR const * const scpc_loadOrb = // NOTE due to the way ACE_DYNAMIC_SERVICE_DIRECTIVE() macro is defined,
  // each parameter CANNOT be split into multiple quoted strings "line1" "nextline" with the expectation that
  // they will be join together. Hence the long parameter 4.
  ACE_DYNAMIC_SERVICE_DIRECTIVE ("testDllOrb",
                                 "DllOrb",
                                 "_make_DllOrb",
"DllOrb -t 1 -ORBGestalt Local -ORBDebugLevel 3 -ORBId testORB -ORBInitRef NameService=file:///tmp/test-ns.ior -ORBDottedDecimalAddresses 1"
                                 );

ACE_TCHAR const * const scpc_unloadOrb = ACE_REMOVE_SERVICE_DIRECTIVE ("testDllOrb");

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,"\nbefore first load process directive\n"));
  if (0 > ACE_Service_Config::process_directive (scpc_loadOrb))
    return -1;

  ACE_OS::sleep(2);

  ACE_DEBUG ((LM_DEBUG,"\nbefore first unload process directive\n"));
  if (0 > ACE_Service_Config::process_directive (scpc_unloadOrb))
    return -1;

  ACE_OS::sleep(2);
  ACE_DEBUG ((LM_DEBUG, "\nMAIN (%P|%t) Lather, Rince, Repeat ...\n"));

  if (0 > ACE_Service_Config::process_directive (scpc_loadOrb))
    return -1;

  ACE_OS::sleep(2);

  if (0 > ACE_Service_Config::process_directive (scpc_unloadOrb))
    return -1;

  return 0;
}
