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
#include "ace/pre.h"

#include "tao/Connection_Purging_Strategy.h"

class TAO_Export TAO_LRU_Connection_Purging_Strategy: public TAO_ULong_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_LRU_Connection_Purging_Strategy (TAO_Resource_Factory* rf);

  /// The destructor
  virtual ~TAO_LRU_Connection_Purging_Strategy ();

protected:

  /// Called when accessing an item from the cache
  virtual void update_item_i (TAO_DESCRIPTOR_INTERFACE* prop,
                              TAO_PURGING_CACHE_ITEM* item);

private:

  ATTRIBUTE_TYPE order_;
};

#include "ace/post.h"
#endif /* TAO_LRU_PURGING_STRATEGY_H */
