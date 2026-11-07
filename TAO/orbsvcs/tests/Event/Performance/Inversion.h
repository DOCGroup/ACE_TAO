/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Inversion.h
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
 */
class EC_Inversion : public EC_Driver
{
public:
  /// Constructor
  EC_Inversion () = default;
  EC_Inversion (EC_Inversion const&) = delete;
  EC_Inversion (EC_Inversion&&) = delete;
  EC_Inversion& operator= (EC_Inversion const&) = delete;
  EC_Inversion& operator= (EC_Inversion&&) = delete;

  // = The EC_Driver methods
  /**
   * Parse the arguments, but override the number of consumers.
   * The options controlling the event types are ignored, and only two
   * consumers are created.
   */
  int parse_args (int& argc, ACE_TCHAR* argv[]) override;

  /**
   * In this test there are two consumers, the high priority consumer
   * receives only events from a single high priority supplier.  The
   * other consumer receives events from a set of low priority
   * suppliers.
   */
  void connect_consumers () override;
  void connect_suppliers () override;

  /// Activate the suppliers at different priorities
  void activate_tasks () override;

private:
  /// If set then both low priority and high priority suppliers
  /// generate the same events.
  bool same_events_ {};
};

#endif /* EC_INVERSION_H */
