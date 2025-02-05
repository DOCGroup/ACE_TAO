/* -*- c++ -*- */
#ifndef JAWS_TPOOL_CONCURRENCY_H
#define JAWS_TPOOL_CONCURRENCY_H

#include "jaws3/Concurrency.h"

class JAWS_Export JAWS_TPOOL_Concurrency : public JAWS_Concurrency_Impl
{
public:
  JAWS_TPOOL_Concurrency ();

  int putq (JAWS_Protocol_Handler *ph);

  int getq (JAWS_Protocol_Handler *&ph);

  static JAWS_TPOOL_Concurrency * instance ()
  {
    return ACE_Singleton<JAWS_TPOOL_Concurrency, ACE_SYNCH_MUTEX>::instance ();
  }

private:
  int number_of_threads_;
  int shutdown_task_;
  int error_;
};

#endif /* JAWS_TPOOL_CONCURRENCY_H */
