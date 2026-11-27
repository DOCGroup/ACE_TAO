#ifndef ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H
#define ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H
#include "Benchmark_Performance.h"

#include <memory>

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Adaptive_Lock_Performance_Test_Base : public Benchmark_Performance
{
public:
  Adaptive_Lock_Performance_Test_Base () = default;
  Adaptive_Lock_Performance_Test_Base (Adaptive_Lock_Performance_Test_Base const &) = delete;
  Adaptive_Lock_Performance_Test_Base (Adaptive_Lock_Performance_Test_Base &&) = delete;
  Adaptive_Lock_Performance_Test_Base &operator= (Adaptive_Lock_Performance_Test_Base const &) = delete;
  Adaptive_Lock_Performance_Test_Base &operator= (Adaptive_Lock_Performance_Test_Base &&) = delete;

  int init (int, ACE_TCHAR *[]) override = 0;
  int fini () override;
  int svc () override;

  int set_lock (std::unique_ptr<ACE_Lock> lock);

private:
  static std::unique_ptr<ACE_Lock> lock_;
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H */
