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

  virtual void connect_consumers (CORBA::Environment &ACE_TRY_ENV);
  virtual void connect_suppliers (CORBA::Environment &ACE_TRY_ENV);
  // In this test there are two consumers, the high priority consumer
  // receives only events from a single high priority supplier.  The
  // other consumer receives events from a set of low priority
  // suppliers.

  virtual void activate_tasks (CORBA::Environment &);
  // Activate the suppliers at different priorities
};

#endif /* EC_INVERSION_H */
