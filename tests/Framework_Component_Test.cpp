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

#include "ace/Service_Config.h"
#include "ace/ARGV.h"
#include "tests/test_config.h"
#include "ace/DLL_Manager.h"

ACE_RCSID(tests, Framework_Component_Test, "$Id$")

int
run_test (u_long unload_mask = 0)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Running test with mask = %s|%s\n"),
              ACE_BIT_ENABLED(unload_mask, ACE_DLL_Manager_Ex::PER_DLL) == 0
              ? ACE_TEXT ("PER_PROCESS") : ACE_TEXT ("PER_DLL"),
              ACE_BIT_ENABLED(unload_mask, ACE_DLL_Manager_Ex::LAZY) == 0
              ? ACE_TEXT ("EAGER") : ACE_TEXT ("LAZY")));

  ACE_DLL_Manager::instance ()->unload_strategy (unload_mask);

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

  // And unload the first one, should *not* unload the dll.
  ACE_Service_Config::process_directive (ACE_TEXT ("remove Server_1"));  

  // Now load another service from the same library.
  ACE_Service_Config::process_directive 
    (ACE_TEXT ("dynamic Server_2 Service_Object * "
               "Framework_Component_DLL:_make_Server_2() 'xxx' "));

  // And unload the first one, should *not* unload the dll.
  //ACE_Service_Config::process_directive (ACE_TEXT ("remove Server_1"));  

  // And unload the second service.  Since the ACE_DLL_Handle will no longer 
  // have any references, the ACE_DLL_Manager will apply it's current unloading
  // strategy and either call ACE_OS::dlclose() immediately, schedule a timeout 
  // the the reactor to call dlclose() some time in the future, or keep the 
  // dll loaded until program termination.
  ACE_Service_Config::process_directive (ACE_TEXT ("remove Server_2"));  

  // Force unloading so we'll be ready for the next test.
  ACE_DLL_Manager::instance ()->unload_strategy (ACE_DLL_Manager_Ex::DEFAULT);

  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Framework_Component_Test"));

  int retval = 0;

  // Use defaults, i.e., per process, eager unloading.
  retval += run_test ();

  // Per process, lazy unloading
  retval += run_test (ACE_DLL_Manager_Ex::LAZY);

  // Per dll, eager unloading
  retval += run_test (ACE_DLL_Manager_Ex::PER_DLL);

  // Per dll, lazy unloading.
  retval += run_test (ACE_DLL_Manager_Ex::PER_DLL | ACE_DLL_Manager_Ex::LAZY);

  ACE_END_TEST;
  return retval == 0 ? 0 : -1;
}
