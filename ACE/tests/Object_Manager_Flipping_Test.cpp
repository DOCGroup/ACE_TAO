// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Object_Manager_Flipping_Test.cpp
//
// = DESCRIPTION

//    Tests the basic function of the ACE_Service_Config in scenarios
//    where the ACE_Object_Manager is being
//    flipped. i.e. ACE::init/ACE::fini() are called in sequence
//    (which can occur with loading DLLs).  More specifically we test
//    that the OM correctly controlls the lifecycle of the default SC
//    instance, which is pointed to by a TSS pointer.
//
// = AUTHOR
//    Iliyan Jeliazkov <iliyan@ociweb.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Object_Manager.h"
#include "ace/OS_Memory.h"
#include "ace/ACE.h"
#include "ace/Service_Config.h"
#include "ace/Intrusive_Auto_Ptr.h"



static u_int *ip;

static const bool is_static_object_manager =
#if defined (ACE_HAS_STATIC_OBJECT_MANAGER)
    true;
#else
    false;
#endif

extern "C"
void
hook1 (void)
{
  delete ip;
  ip = 0;
}


int run_main (int, ACE_TCHAR *[])
{

  // Causing the creation of a SC instance and the corresponding TSS
  // key.  It is not registered with the Object Manager, but beware -
  // OM finalization will destroy it too.
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> p0 (ACE_Service_Config::instance ());
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> p1;
  u_int errors = 0;

  // ...
  {
    ACE::init ();
    ACE_START_TEST (ACE_TEXT ("Object_Manager_Flipping_Test"));


    // If hook1 never gets called, this will show up as a memory leak.
    ACE_NEW_RETURN (ip,
                    u_int,
                    -1);

    if (ACE_OS::atexit (hook1) != 0)
      {
        ++errors;
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("ACE_OS::atexit () returned non-zero!!!!")));
      }

    // Obtain a SC instance which will be later used to compare with others.
    p1 = ACE_Service_Config::instance ();

    // ACE_TEST_ASSERT uses Log_Msg::instance() and needs to be done only
    // after ACE_START_TEST

    // Additional ACE::init() should not have changed  the context
    ACE_TEST_ASSERT (p0 == p1);

    // It should appear open
    ACE_TEST_ASSERT (!p0->is_opened ());

    ACE_END_TEST;


    // ACE::fini() destroys the SC (unmanaged) singleton ...
    // Next time we ask for one, it will be a different instance.
    ACE::fini ();
  }

  // This is a legitimate test, but more importantly an
  // attemp to dereference p1 should succeed. If SC's TSS
  // was not cleaned correctly this will SEGV. As will the
  // following ACE::init, as it tries to use the SC instance.

  if (ACE_Service_Config::instance ()->is_opened ())
      ++errors;

  // Not using ACE_TEST_ASSERT because ACE is not initialized yet.

  {
    ACE::init();
    ACE_START_TEST (ACE_TEXT ("Object_Manager_Flipping_Test"));

    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> p2 (ACE_Service_Config::instance ());

    // ACE_TEST_ASSERT uses Log_Msg::instance() and needs to be done only
    // after ACE_START_TEST
    // An attempt to dereference should be fine.
    ACE_TEST_ASSERT (!p2->is_opened ());

    ACE_END_TEST;
    ACE::fini();   // Flipped twice
  }

  return errors == 0 ? 0 : 1;

}
