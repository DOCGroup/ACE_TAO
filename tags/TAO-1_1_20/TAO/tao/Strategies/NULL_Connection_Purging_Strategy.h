// ================================================================
/**
 *  @file   NULL_Connection_Purging_Strategy.h
 *
 *  $Id$
 *
 *  @author Chad Elliott (elliott_c@ociweb.com)
 */
// ================================================================

#ifndef TAO_NULL_PURGING_STRATEGY_H
#define TAO_NULL_PURGING_STRATEGY_H
#include "ace/pre.h"

#include "strategies_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Connection_Purging_Strategy.h"


/**
 * @class TAO_NULL_Connection_Purging_Strategy
 *
 * @brief The null connection purging strategy
 *
 * This class does not do anything.
 */

class TAO_Strategies_Export TAO_NULL_Connection_Purging_Strategy: public TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_NULL_Connection_Purging_Strategy (int cache_maximum);

  /// The destructor
  virtual ~TAO_NULL_Connection_Purging_Strategy (void);

  /// Does nothing.
  virtual void update_item (TAO_Transport* transport);

  /// Returns -1 to signify no maximum
  virtual int cache_maximum (void) const;
};

#include "ace/post.h"
#endif /* TAO_NULL_PURGING_STRATEGY_H */
