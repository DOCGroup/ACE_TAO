/* -*- c++ -*- */
#ifndef JAWS_CONCURRENCY_T_H
#define JAWS_CONCURRENCY_T_H

#include "ace/config-lite.h"

class JAWS_Protocol_Handler;

/// Concurrency bridge abstraction using the Concrete Bridge pattern.
///
/// When CONCURRENCY_IMPL is JAWS_Concurrency_Impl, this class has the classic
/// abstraction role. When it is a concrete implementor, the class acts more
/// like an adapter, avoiding the Bridge pattern's polymorphism overhead.
template <class CONCURRENCY_IMPL>
class JAWS_Concurrency_Bridge
{
public:
  explicit JAWS_Concurrency_Bridge (CONCURRENCY_IMPL *impl = nullptr);

  int putq (JAWS_Protocol_Handler *ph);
  int getq (JAWS_Protocol_Handler *&ph);

  void shutdown ();

protected:
  CONCURRENCY_IMPL *impl_;
};

#include "jaws3/Concurrency_T.cpp"

#endif /* JAWS_CONCURRENCY_T_H */
