// $Id$

#include "Worker_T_Test.h"
#include "cuts/Worker_T.h"
#include "cuts/Worker.h"
#include "cuts/Action_T.h"
#include "cuts/WML_Macros.h"
#include "cuts/Auto_Release_Ptr.h"

#include "ace/OS_NS_stdlib.h"
#include "ace/streams.h"

CUTS_TEST_EXPORT (CUTS_Worker_T_Test)

namespace CUTS_TEST
{
  static int test_value_ = 0;
}

//=============================================================================
/*
 * Dummy_Worker
 */
//=============================================================================

class Dummy_Worker :
  public CUTS_Worker,
  public CUTS_Worker_T <Dummy_Worker>
{
public:
  class First_Action : public CUTS_Action_T <Dummy_Worker>
  {
  public:
    First_Action (Dummy_Worker & worker)
      : CUTS_Action_T <Dummy_Worker> (worker)
    {  }

    virtual ~First_Action (void)
    {  }

    virtual void execute (void) const
    {
      this->parent_.first_action ();
    }

    CUTS_DECLARE_ACTION_FACTORY (Dummy_Worker, First_Action);
  };

  class Second_Action : public CUTS_Action_T <Dummy_Worker>
  {
  public:
    Second_Action (Dummy_Worker & worker)
      : CUTS_Action_T <Dummy_Worker> (worker)
    {  }

    virtual ~Second_Action (void)
    {  }

    virtual void execute (void) const
    {
      this->parent_.second_action ();
    }

    CUTS_DECLARE_ACTION_FACTORY (Dummy_Worker, Second_Action);
  };

  Dummy_Worker (void) { }
  virtual ~Dummy_Worker (void) { }

  void first_action (void)
  {
    CUTS_TEST::test_value_ = 1;
  }

  void second_action (void)
  {
    CUTS_TEST::test_value_ = 2;
  }

protected:
  CUTS_ACTION_TABLE_DECLARE ();
};

CUTS_ACTION_TABLE_BEGIN (Dummy_Worker, CUTS_Worker)
  CUTS_ACTION_TABLE_ENTRY ("First_Action", Dummy_Worker::First_Action);
  CUTS_ACTION_TABLE_ENTRY ("Second_Action", Dummy_Worker::Second_Action);
CUTS_ACTION_TABLE_END (Dummy_Worker)

//=============================================================================
/*
 * CUTS_Worker_T_Test
 */
//=============================================================================

CUTS_TEST_IMPL_BEGIN (CUTS_Worker_T_Test, "Worker_T_Test")
  CUTS_ADD_UNIT_TEST (Unit_Test_Dummy_Worker)
  CUTS_ADD_UNIT_TEST (Unit_Test_Action_Table)
  CUTS_ADD_UNIT_TEST (Unit_Test_Action_Iterator)
CUTS_TEST_IMPL_END (CUTS_Worker_T_Test)

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

  CUTS_VERIFY_TEST (worker.action_count () != 2,
                    "expected action_count () == 2; action_count () == "
                    << worker.action_count ());

  // Test the parent () method.
  long parent = ACE_OS::rand ();
  worker.parent (parent);
  CUTS_VERIFY_TEST (worker.parent () != parent,
                    "expected parent () == " << parent);
  return 0;
}

//=============================================================================
/*
 * Unit_Test_Action_Table
 */
//=============================================================================

CUTS_UNIT_TEST_IMPL (Unit_Test_Action_Table, "Action_Table");

CUTS_UNIT_TEST_RUN_IMPL (Unit_Test_Action_Table)
{
  Dummy_Worker worker;

  // This should always return 0 with for workers that are subclasses
  // of CUTS_Worker objects.
  CUTS_VERIFY_TEST (worker.create_action ("My_Action") != 0,
                    "expected create_action () = 0");

  // Verify the actions can be created.
  CUTS_Auto_Release_Ptr <CUTS_Action> action;
  action.reset (worker.create_action ("First_Action"));
  CUTS_VERIFY_TEST (action.get () == 0,
                    "failed to create First_Action");

  // Execute the action then release it's resources.
  action->execute ();
  CUTS_VERIFY_TEST (CUTS_TEST::test_value_ != 1,
                    "expected test_value_ == 1");

  action.reset (worker.create_action ("Second_Action"));
  CUTS_VERIFY_TEST (action.get () == 0,
                    "failed to create Second_Action");

  // Execute the action then release it's resources.
  action->execute ();
  CUTS_VERIFY_TEST (CUTS_TEST::test_value_ != 2,
                    "expected test_value_ == 2");
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

  // Test the default iterator.
  CUTS_Action_Iterator * iter = worker.begin ();
  CUTS_VERIFY_TEST (iter == 0,
                    "begin () return bad iterator");

  ACE_CString action_name = iter->action_name ();
  CUTS_VERIFY_TEST ((action_name != "First_Action" &&
                     action_name != "Second_Action"),
                    "action_name () is not valid");

  // Test the creation of the action from the iterator.
  CUTS_Auto_Release_Ptr <CUTS_Action> action;
  action.reset (iter->create_action ());
  CUTS_VERIFY_TEST (action.get () == 0,
                    "failed to create" << action_name.c_str ());

  // Execute the action and verify the <test_value_>.
  action->execute ();
  if (action_name == "First_Action")
  {
    CUTS_VERIFY_TEST (CUTS_TEST::test_value_ != 1,
                      "failed to execute First_Action");
  }
  else
  {
    CUTS_VERIFY_TEST (CUTS_TEST::test_value_ != 2,
                      "failed to execute Second_Action");
  }

  // Test the advance () operator.
  iter->advance ();
  CUTS_VERIFY_TEST (iter->done (),
                    "iterator should not be done () after first advance ()");
  CUTS_VERIFY_TEST ((action_name == "First_Action" &&
                     iter->action_name () != "Second_Action") ||
                    (action_name == "Second_Action" &&
                     iter->action_name () != "First_Action"),
                     "advance operation failed");

  // Test the reverse () operator.
  iter->reverse ();
  CUTS_VERIFY_TEST (iter->done (),
                    "iterator should not be done () after reverse ()");
  CUTS_VERIFY_TEST ((action_name == "First_Action" &&
                     iter->action_name () != "First_Action") ||
                    (action_name == "Second_Action" &&
                     iter->action_name () != "Second_Action"),
                     "reverse operation failed");

  // We do not have to create a "Second_Action" since we have
  // already tested that creation operations above.
  return 0;
}
