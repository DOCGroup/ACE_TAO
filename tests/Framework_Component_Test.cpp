// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests both the ACE_Framework_Compondent and ACE_Repository.
//    Since Framework Components are singletons that can live in dlls loaded
//    via the Service Configurator framework, this test uses that framework
//    to load services from a dll that has a singleton based on ACE_DLL_Singleton.
//    When the dll is finally ready to be unloaded, the singleton will be 
//    automatically cleaned up just-in-time.
//
// = AUTHOR
//    Don Hinton <dhinton@ieee.org>
//
// ============================================================================

//#include "Framework_Component_DLL.h"
#include "ace/Service_Config.h"
#include "ace/ARGV.h"
#include "tests/test_config.h"

ACE_RCSID(tests, Framework_Component_Test, "$Id$")

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Framework_Component_Test"));

  // Now, let the ACE Service Configurator framework load our service from a
  // dll, which contains a singleton.
  ACE_ARGV args;
  args.add (ACE_TEXT ("Framework_Component_Test"));
  args.add (ACE_TEXT ("-n"));
  args.add (ACE_TEXT ("-d"));
  args.add (ACE_TEXT ("-S"));
  args.add (ACE_TEXT ("\"dynamic Server_1 Service_Object * "
                      "Framework_Component_DLL:_make_Server_1() 'xxx' \""));

  // Load it, should load a dll.
  ACE_Service_Config::open (args.argc (), args.argv ());

  // Now load another service from the same library.
  ACE_Service_Config::process_directive 
    (ACE_TEXT ("dynamic Server_2 Service_Object * "
               "Framework_Component_DLL:_make_Server_2() 'xxx' "));

  // And unload the first one, should *not* unload the dll.
  ACE_Service_Config::process_directive (ACE_TEXT ("remove Server_1"));  

  // And unload the second service.  Since the ACE_DLL_Handle will no longer 
  // have any references, the ACE_DLL_Manager will apply it's current unloading
  // strategy and either call ACE_OS::dlclose() immediately, schedule a timeout 
  // the the reactor to call dlclose() some time in the future, or keep the 
  // dll loaded until program termination.
  ACE_Service_Config::process_directive (ACE_TEXT ("remove Server_2"));  

  ACE_END_TEST;
  return 0;
}
