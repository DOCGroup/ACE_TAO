// $Id$

#include "Worker_Test.h"
#include "cuts/Worker.h"
#include "ace/OS_NS_stdlib.h"

CUTS_TEST_EXPORT (CUTS_Worker_Test)

//=============================================================================
/*
 * Dummy_Worker
 */
//=============================================================================

class Dummy_Worker : public CUTS_Worker
{
public:
  Dummy_Worker (void) { }
  virtual ~Dummy_Worker (void) { }
};

//=============================================================================
/*
 * CUTS_Worker_Test
 */
//=============================================================================

CUTS_TEST_IMPL_BEGIN (CUTS_Worker_Test, "Worker_Test")
  CUTS_ADD_UNIT_TEST (Unit_Test_Dummy_Worker)
  CUTS_ADD_UNIT_TEST (Unit_Test_Action_Iterator)
CUTS_TEST_IMPL_END (CUTS_Worker_Test)

//=============================================================================
/*
 * Unit_Test_Dummy_Worker
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Unit_Test_Dummy_Worker, "Dummy_Worker")

CUTS_UNIT_TEST_RUN_IMPL (Unit_Test_Dummy_Worker)
{
  Dummy_Worker worker;

  // Verify the default settings of the <worker>.
  CUTS_VERIFY_TEST (worker.parent () != -1,
                    "expected parent () == -1");

  CUTS_VERIFY_TEST (worker.counter () != 0,
                    "expected counter () == 0");

  CUTS_VERIFY_TEST (worker.action_count () != 0,
                    "expected action_count () == 0");

  // Test the parent () method.
  long parent = ACE_OS::rand ();
  worker.parent (parent);
  CUTS_VERIFY_TEST (worker.parent () != parent,
                    "expected parent () == " << parent);

  // This should always return 0 with for workers that are subclasses
  // of CUTS_Worker objects.
  CUTS_VERIFY_TEST (worker.create_action ("My_Action") != 0,
                    "expected create_action () = 0");
  return 0;
}

//=============================================================================
/*
 * Unit_Test_Action_Iterator
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Unit_Test_Action_Iterator, "Action_Iterator");

CUTS_UNIT_TEST_RUN_IMPL (Unit_Test_Action_Iterator)
{
  Dummy_Worker worker;
  CUTS_Action_Iterator * iter = worker.begin ();

  CUTS_VERIFY_TEST (!iter->action_name ().empty (),
                    "expected action_name () to be empty");

  CUTS_VERIFY_TEST (iter->create_action () != 0,
                    "expected create_action () == 0");

  CUTS_VERIFY_TEST (!iter->done (),
                    "expected iter to be done when created");

  iter->advance ();
  CUTS_VERIFY_TEST (!iter->done (),
                    "expected iter to be done after advance ()");

  iter->reverse ();
  CUTS_VERIFY_TEST (!iter->done (),
                    "expected iter to be done after reverse ()");

  // Release the resources.
  iter->release ();
  return 0;
}
