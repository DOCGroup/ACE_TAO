// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Token.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Token_Test : public Benchmark_Performance
{
public:
  virtual int svc (void);

private:
  static ACE_Token token;
};

ACE_Token Token_Test::token;

int
Token_Test::svc (void)
{
  // Extract out the unique thread-specific value to be used as an
  // index...
  int ni = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      token.acquire ();
      performance_test_options.thr_work_count[ni]++;
      buffer++;
      token.release ();
    }
  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Token_Test)
ACE_SVC_FACTORY_DEFINE  (Token_Test)

// ACE_Service_Object_Type tok (&token_test, "Token_Test");
#endif /* ACE_HAS_THREADS */
