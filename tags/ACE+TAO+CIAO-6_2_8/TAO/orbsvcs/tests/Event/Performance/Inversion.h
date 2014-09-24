/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Inversion.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_INVERSION_H
#define EC_INVERSION_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC_Inversion
 *
 * @brief Measure priority inversions in the EC.
 *
 */
class EC_Inversion : public EC_Driver
{
public:
  /// Constructor
  EC_Inversion (void);

  // = The EC_Driver methods
  /**
   * Parse the arguments, but override the number of consumers.
   * The options controlling the event types are ignored, and only two
   * consumers are created.
   */
  int parse_args (int &argc, ACE_TCHAR *argv[]);

  /**
   * In this test there are two consumers, the high priority consumer
   * receives only events from a single high priority supplier.  The
   * other consumer receives events from a set of low priority
   * suppliers.
   */
  virtual void connect_consumers (void);
  virtual void connect_suppliers (void);

  /// Activate the suppliers at different priorities
  virtual void activate_tasks (void);

private:
  /// If set then both low priority and high priority suppliers
  /// generate the same events.
  int same_events_;
};

#endif /* EC_INVERSION_H */
