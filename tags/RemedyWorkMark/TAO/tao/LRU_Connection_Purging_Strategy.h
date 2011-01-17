// -*- C++ -*-

// ================================================================
/**
 *  @file   LRU_Connection_Purging_Strategy.h
 *
 *  $Id$
 *
 *  @author Chad Elliott (elliott_c@ociweb.com)
 */
// ================================================================

#ifndef TAO_LRU_PURGING_STRATEGY_H
#define TAO_LRU_PURGING_STRATEGY_H

#include /**/ "ace/pre.h"

#include "tao/Connection_Purging_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LRU_Connection_Purging_Strategy
 *
 * @brief The Least Recently Used connection purging strategy
 *
 * This class maintains it's own count which is applied to the
 * item passed in.  This way, the least recently used transport
 * has the smallest ordering number and will therefore be purged
 * first.
 */
class TAO_Export TAO_LRU_Connection_Purging_Strategy
  : public TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_LRU_Connection_Purging_Strategy (int cache_maximum);

  /// The destructor
  virtual ~TAO_LRU_Connection_Purging_Strategy (void);

  /// Called when accessing an item from the cache
  virtual void update_item (TAO_Transport& transport);

private:
  /// The ordering information for each transport in the cache
  unsigned long order_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_LRU_PURGING_STRATEGY_H */
