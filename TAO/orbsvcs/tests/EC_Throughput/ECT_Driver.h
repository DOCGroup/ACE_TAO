/* -*- C++ -*- */
//
// ============================================================================
//
// = DESCRIPTION
//   Abstract base class for the test driver, this let us implement a
//   collocated and a non-collocated test.
//
// ============================================================================

#ifndef ECT_DRIVER_H
#define ECT_DRIVER_H

#include "tao/orbconf.h"
#include "ace/Stats.h"
#include "ace/CORBA_macros.h"
#include "ace/Throughput_Stats.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class Environment;
}
TAO_END_VERSIONED_NAMESPACE_DECL

class ECT_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECT_Driver () = default;
  virtual ~ECT_Driver () = default;

  ECT_Driver (ECT_Driver const&) = delete;
  ECT_Driver (ECT_Driver&&) = delete;
  ECT_Driver& operator= (ECT_Driver const&) = delete;
  ECT_Driver& operator= (ECT_Driver&&) = delete;

  virtual void shutdown_consumer () = 0;
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.
};

#endif /* ECT_CONSUMER_H */
