// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Synch.h"
#include "Baseline_Test.h"

class ACE_Svc_Export Baseline_Base_Test : public Baseline_Test_Base
{
public:
  virtual int acquire ();
  virtual int release ();
  // These two method are used to test try_acquire performance.

  virtual int test_acquire_release ();
  virtual int test_try_lock ();
  // Real test methods.
};

int
Baseline_Base_Test::acquire ()
{
  return 0;
}

int
Baseline_Base_Test::release ()
{
  return 0;
}

int
Baseline_Base_Test::test_acquire_release ()
{
  baseline_options.start_inc_timer ();

  for (; baseline_options.inc_loop_counter () ; )
    ;

  baseline_options.stop_inc_timer ();
  return 0;
}

int
Baseline_Base_Test::test_try_lock ()
{
  baseline_options.start_inc_timer ();

  for (; baseline_options.inc_loop_counter () ; )
    ;

  baseline_options.stop_inc_timer ();
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Baseline_Base_Test)
ACE_SVC_FACTORY_DEFINE (Baseline_Base_Test)
