// -*- C++ -*-
#ifndef JAWS_CONCURRENCY_H
#define JAWS_CONCURRENCY_H

#include "ace/Task.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"

#include "jaws3/Export.h"
#include "jaws3/Protocol_Handler.h"


using JAWS_CONCURRENCY_TASK = ACE_Task<ACE_SYNCH>;

class JAWS_Export JAWS_Concurrency_Impl : public JAWS_CONCURRENCY_TASK
{
public:
  JAWS_Concurrency_Impl () = default;
  virtual ~JAWS_Concurrency_Impl () = default;
  JAWS_Concurrency_Impl (JAWS_Concurrency_Impl const &) = delete;
  JAWS_Concurrency_Impl (JAWS_Concurrency_Impl &&) = delete;
  JAWS_Concurrency_Impl &operator= (JAWS_Concurrency_Impl const &) = delete;
  JAWS_Concurrency_Impl &operator= (JAWS_Concurrency_Impl &&) = delete;

  virtual int putq (JAWS_Protocol_Handler *ph) = 0;

  virtual int getq (JAWS_Protocol_Handler *&ph) = 0;

  int svc () override;
};


// Include the templates here.
#include "jaws3/Concurrency_T.h"


template<>
class JAWS_Export JAWS_Concurrency_Bridge<JAWS_Concurrency_Impl>
{
public:
  explicit JAWS_Concurrency_Bridge (JAWS_Concurrency_Impl *impl = nullptr);

  int putq (JAWS_Protocol_Handler *ph);
  int getq (JAWS_Protocol_Handler *&ph);

  void shutdown ();

protected:
  JAWS_Concurrency_Impl *impl_;
};


#ifndef JAWS_CONCURRENCY_CONCRETE_IMPL
#define JAWS_CONCURRENCY_CONCRETE_IMPL JAWS_Concurrency_Impl
#endif /* JAWS_CONCURRENCY_CONCRETE_IMPL */


class JAWS_Export JAWS_Concurrency
  : public JAWS_Concurrency_Bridge<JAWS_CONCURRENCY_CONCRETE_IMPL>
{
public:
  static JAWS_Concurrency * instance ()
  {
    return ACE_Singleton<JAWS_Concurrency, ACE_SYNCH_MUTEX>::instance ();
  }
};


#endif /* JAWS_CONCURRENCY_H */
