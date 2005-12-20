/* -*- C++ -*- */
// $Id$
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
  virtual ~ECT_Driver (void);

  virtual void shutdown_consumer (void* consumer_cookie
                                  ACE_ENV_ARG_DECL_NOT_USED) = 0;
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.
};

#endif /* ECT_CONSUMER_H */
