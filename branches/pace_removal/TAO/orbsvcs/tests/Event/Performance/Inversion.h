/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Inversion
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_INVERSION_H
#define EC_INVERSION_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EC_Inversion : public EC_Driver
{
  //
  // = TITLE
  //   Measure priority inversions in the EC.
  //
  // = DESCRIPTION
  //
public:
  EC_Inversion (void);
  // Constructor

  // = The EC_Driver methods
  int parse_args (int &argc, char *argv[]);
  // Parse the arguments, but override the number of consumers.
  // The options controlling the event types are ignored, and only two
  // consumers are created.

  virtual void connect_consumers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  virtual void connect_suppliers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // In this test there are two consumers, the high priority consumer
  // receives only events from a single high priority supplier.  The
  // other consumer receives events from a set of low priority
  // suppliers.

  virtual void activate_tasks (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Activate the suppliers at different priorities

private:
  int same_events_;
  // If set then both low priority and high priority suppliers
  // generate the same events.
};

#endif /* EC_INVERSION_H */
