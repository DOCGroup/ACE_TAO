// -*- C++ -*-

// ================================================================
/**
 *  @file   LFU_Connection_Purging_Strategy.h
 *
 *  $Id$
 *
 *  @author Chad Elliott (elliott_c@ociweb.com)
 */
// ================================================================

#ifndef TAO_LFU_PURGING_STRATEGY_H
#define TAO_LFU_PURGING_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/Strategies/strategies_export.h"
#include "tao/Connection_Purging_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LFU_Connection_Purging_Strategy
 *
 * @brief The Least Frequently Used connection purging strategy
 *
 * This class increments the order information on each item passed
 * in.  This way, the least frequently used transport has the
 * smallest ordering number and will therefore be purged first.
 */

class TAO_Strategies_Export TAO_LFU_Connection_Purging_Strategy:
                            public TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_LFU_Connection_Purging_Strategy (int cache_maximum);

  /// Called when accessing an item from the cache
  virtual void update_item (TAO_Transport& transport);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_LFU_PURGING_STRATEGY_H */
