/* -*- c++ -*- */
#ifndef JAWS_THYBRID_CONCURRENCY_H
#define JAWS_THYBRID_CONCURRENCY_H

#include "jaws3/Concurrency.h"
#include <atomic>

class JAWS_Export JAWS_THYBRID_Concurrency : public JAWS_Concurrency_Impl
{
public:
  JAWS_THYBRID_Concurrency ();

  int putq (JAWS_Protocol_Handler *ph) override;

  int getq (JAWS_Protocol_Handler *&ph) override;

  static JAWS_THYBRID_Concurrency * instance ()
  {
    return ACE_Singleton<JAWS_THYBRID_Concurrency, ACE_SYNCH_MUTEX>
           ::instance ();
  }

private:
  std::atomic<int> getting_ {0};
  int min_number_of_threads_;
  int max_number_of_threads_;
  int shutdown_task_;
  int error_;
};

#endif /* JAWS_THYBRID_CONCURRENCY_H */
