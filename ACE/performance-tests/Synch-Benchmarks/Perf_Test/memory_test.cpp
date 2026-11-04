#define  ACE_BUILD_SVC_DLL
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#include <memory>

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Memory_Test : public Benchmark_Performance
{
public:
  int svc () override;
};

int
Memory_Test::svc ()
{
  int const ni = this->thr_id ();
  size_t const length = performance_test_options.msg_size ();
  std::unique_ptr<char[]> from = std::make_unique<char[]> (length);
  std::unique_ptr<char[]> to = std::make_unique<char[]> (length);

  synch_count = 1;

  while (!this->done ())
    {
      ACE_OS::memcpy (to.get (), from.get (), length);
      performance_test_options.thr_work_count[ni]++;
    }
  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Memory_Test)
ACE_SVC_FACTORY_DEFINE  (Memory_Test)

// ACE_Service_Object_Type mt (&memory_test, "Memory_Test");
#endif /* ACE_HAS_THREADS */
