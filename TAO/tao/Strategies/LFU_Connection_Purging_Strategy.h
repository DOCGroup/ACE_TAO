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
#include "ace/pre.h"

#include "strategies_export.h"
#include "tao/Connection_Purging_Strategy.h"

class TAO_Strategies_Export TAO_LFU_Connection_Purging_Strategy:
                            public TAO_ULong_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_LFU_Connection_Purging_Strategy (TAO_Resource_Factory* rf);

  /// The destructor
  virtual ~TAO_LFU_Connection_Purging_Strategy ();

protected:

  /// Called when accessing an item from the cache
  virtual void update_item_i (TAO_DESCRIPTOR_INTERFACE* prop,
                              TAO_PURGING_CACHE_ITEM* item);

};

#include "ace/post.h"
#endif /* TAO_LFU_PURGING_STRATEGY_H */
