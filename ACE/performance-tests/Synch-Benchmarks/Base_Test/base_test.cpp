#define ACE_BUILD_SVC_DLL

#include "Baseline_Test.h"

# if defined (ACE_HAS_THREADS)

#include "ace/Log_Msg.h"

class ACE_Svc_Export Baseline_Base_Test : public Baseline_Test_Base
{
public:
  Baseline_Base_Test (Baseline_Base_Test const &) = delete;
  Baseline_Base_Test (Baseline_Base_Test &&) = delete;
  Baseline_Base_Test &operator= (Baseline_Base_Test const &) = delete;
  Baseline_Base_Test &operator= (Baseline_Base_Test &&) = delete;
  Baseline_Base_Test () = default;

  int acquire () override;
  int release () override;
  // These two method are used to test try_acquire performance.

  int test_acquire_release () override;
  int test_try_lock () override;
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

#endif  /* ACE_HAS_THREADS */
