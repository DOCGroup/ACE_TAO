// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests both the ACE_Framework_Compondent and ACE_Repository.
//
// = AUTHOR
//    Don Hinton <dhinton@ieee.org>
//
// ============================================================================

#include "Framework_Component_DLL.h"
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
  //args.add (ACE_TEXT ("-f"));
  //args.add (ACE_TEXT ("Framework_Component_Test.conf"));
  args.add (ACE_TEXT ("-S"));
  args.add (ACE_TEXT ("\"dynamic Server_1 Service_Object * "
                      "Framework_Component_DLL:_make_Server_1() 'xxx' \""));

  // Load it, should load a dll.
  ACE_Service_Config::open (args.argc (), args.argv ());

  // Now that the dll has been loaded, the singleton is available.
  Simple_Service *ss = SS_SINGLETON::instance ();
  ACE_DEBUG ((LM_DEBUG, 
              ACE_TEXT ("Simple_Service lives in library: %s\n"),
              ss->dll_name ()));

  // Now load another service from the same library.
  ACE_Service_Config::process_directive 
    (ACE_TEXT ("dynamic Server_2 Service_Object * "
               "Framework_Component_DLL:_make_Server_2() 'xxx' "));

  // And unload the first one, should *not* unload the dll.
  ACE_Service_Config::process_directive (ACE_TEXT ("remove Server_1"));  

  // Make sure our singlton is still happy..
  ACE_DEBUG ((LM_DEBUG, 
              ACE_TEXT ("Simple_Service is still alive in library: %s\n"),
              ss->dll_name ()));

  //ACE_Service_Config::close ();

  ACE_END_TEST;
  return 0;
}
