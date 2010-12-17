// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Intrusive_Auto_Ptr_Test
//
// = DESCRIPTION
//    This test verifies the functionality of the <ACE_Intrusive_Auto_Ptr>
//    implementation.
//
// = AUTHOR
//    Iliyan Jeliazkov <iliyan@ociweb.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Intrusive_Auto_Ptr.h"
#include "ace/Thread_Manager.h"



class One {

  static bool released;

  int m2;
  int ref;

public:
  One (int refcount): ref(refcount)
  {
    released = false;
  }

  ~One ()
  {
    released = true;
  }

  bool has_refs (int howmany)
  {
    return this->ref == howmany;
  }

  static bool was_released (void)
  {
    return released;
  }

  static void intrusive_add_ref (One *);
  static void intrusive_remove_ref (One *);
};

bool One::released = true;

void
One::intrusive_add_ref (One *one) {
  one->ref++;
}

void
One::intrusive_remove_ref (One *one) {
  one->ref--;
  if (one->ref == 0)
    delete one;
}



int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Intrusive_Auto_Ptr_Test"));


  One *theone (new One(0));

  {
    ACE_TEST_ASSERT (theone->has_refs (0));
    ACE_TEST_ASSERT (!One::was_released ());

    ACE_Intrusive_Auto_Ptr<One> ip2(theone);

    {
      ACE_TEST_ASSERT (theone->has_refs (1));
      ACE_TEST_ASSERT (!One::was_released ());

      ACE_Intrusive_Auto_Ptr<One> ip2(theone);
      ACE_TEST_ASSERT (theone->has_refs (2));
      ACE_TEST_ASSERT (!One::was_released ());
    }

    ACE_TEST_ASSERT (theone->has_refs (1));
    ACE_TEST_ASSERT (!One::was_released ());
  }

  ACE_TEST_ASSERT (One::was_released());

  ACE_END_TEST;
  return 0;
}
