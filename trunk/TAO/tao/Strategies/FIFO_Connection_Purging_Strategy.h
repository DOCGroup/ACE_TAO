// ================================================================
/**
 *  @file   FIFO_Connection_Purging_Strategy.h
 *
 *  $Id$
 *
 *  @author Chad Elliott (elliott_c@ociweb.com)
 */
// ================================================================

#ifndef TAO_FIFO_PURGING_STRATEGY_H
#define TAO_FIFO_PURGING_STRATEGY_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "strategies_export.h"
#include "tao/Connection_Purging_Strategy.h"

// ****************************************************************

/**
 * @class TAO_FIFO_Connection_Purging_Strategy
 *
 * @brief The First In First Out connection purging strategy
 *
 * This class maintains it's own count which is only applied to the
 * item passed if it has not yet been cached.  This way, each successive
 * transport has a larger ordering number than the previous.  This will
 * cause the first transport to be purged first.
 */

class TAO_Strategies_Export TAO_FIFO_Connection_Purging_Strategy:
                            public TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_FIFO_Connection_Purging_Strategy (int cache_maximum);

  /// The destructor
  virtual ~TAO_FIFO_Connection_Purging_Strategy (void);

  /// Called when accessing an item from the cache
  virtual void update_item (TAO_Cache_IntId& int_id);

private:
  /// The ordering information for each transport in the cache
  unsigned long order_;
};

#include "ace/post.h"
#endif /* TAO_FIFO_PURGING_STRATEGY_H */
