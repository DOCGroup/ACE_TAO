#include "test_config.h"

#include "ace/TSS_T.h"
#include "ace/Task.h"
#include "ace/Barrier.h"

struct X : private ACE_Copy_Disabled
{
  static int count_;

  X() { ++count_; }
  ~X() { --count_; }
};

int X::count_;

ACE_Barrier barrier (2);

struct Task : ACE_Task_Base
{
  explicit Task (ACE_TSS<X> *tss) : tss_(tss) {}

  int svc (void)
  {
    X *x = *tss_;
    ACE_UNUSED_ARG (x);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d X objects created\n"), X::count_));
    barrier.wait ();
    // tss deleted here
    barrier.wait ();
    return 0;
  }

  ACE_TSS<X> *const tss_;
};

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("TSS_Leak_Test"));

  ACE_TSS<X> *const tss = new ACE_TSS<X>;
  X *x = *tss;
  ACE_UNUSED_ARG (x);
  Task task (tss);
  task.activate ();
  barrier.wait ();
  delete tss;
  barrier.wait ();
  task.wait ();

  if (X::count_)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: %d X objects (should be 0)\n"),
                  X::count_));
    }

  ACE_END_TEST;
  return X::count_;
}
