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

//#include "ace/Framework_Component.h"
#include "ace/Service_Config.h"
//#include "ace/OS.h"
#include "ace/ARGV.h"
#include "tests/test_config.h"
//#include "tests/Framework_Component_Test.h"

ACE_RCSID(tests, Framework_Component_Test, "$Id$")

// This is the original test, that tests the basic functions of the
// repository.
void repository_test (void)
{
  //Server s;
  //ACE_ARGV a("xxx xxx");
  //s.init(a.argc (), a.argv ());

  // Create an instance of Simple_Service that will be managed
  // by the ACE_Framework_Repository and keep a pointer to
  // instance.
  //Simple_Service *first = SS_SINGLETON::instance ();

  //FWCT_DLL_Singleton_Adapter_T <Simple_Service> *ss = SS_SINGLETON::instance ();

  //ACE_DEBUG ((LM_DEBUG, 
  //            ACE_LIB_TEXT ("And the name of the dll is: %s\n"),
  //            ss->dll_name ()));

  // Now, close down the repository.
  ACE_Framework_Repository::close_singleton ();

  ACE_Framework_Repository::instance ();

  // Now get another pointer to Simple_Service and make sure
  // that it's different.  We could also test other pointers
  // like ACE_Reactor, ACE_Proactor, or the repository itself
  // if we wanted.
  //Simple_Service *second = SS_SINGLETON::instance ();

  //if (first == second)
  //  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Pointer are equal, but shouldn't be.\n")));

  // Close down the repository again since we are writing debug messages.
  // And want all of them to go to the log.
  ACE_Framework_Repository::close_singleton ();

}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Framework_Component_Test"));

  // First run the regular repository test (this was actually all the
  // test was originally).
  //repository_test ();
  
  // Now, let the ACE Service Configurator framework load our service from a
  // dll, which contains a singleton.
  ACE_ARGV args;
  args.add (ACE_TEXT ("Framework_Component_Test"));
  args.add (ACE_TEXT ("-n"));
  args.add (ACE_TEXT ("-d"));
  //args.add (ACE_TEXT ("-f"));
  //args.add (ACE_TEXT ("Framework_Component_Test.conf"));
  args.add (ACE_TEXT ("-S"));
  args.add (ACE_TEXT ("\"dynamic Server Service_Object * Framework_Component_DLL:_make_Server() 'xxx' \""));

  // Load it, should load a dll.
  ACE_Service_Config::open (args.argc (), args.argv ());

  // And unload it, should unload the dll.
  ACE_Service_Config::process_directive (ACE_TEXT ("\"remove Server\""));  

  //ACE_Service_Config::close ();

  ACE_END_TEST;
  return 0;
}
