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

#include "tao/corba.h"
#include "ace/Stats.h"

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
                                  TAO_ENV_ARG_DECL_NOT_USED) = 0;
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.
};

#if defined (__ACE_INLINE__)
#include "ECT_Driver.i"
#endif /* __ACE_INLINE__ */

#endif /* ECT_CONSUMER_H */
