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
#include "tao/Connection_Purging_Strategy.h"

class TAO_Strategies_Export TAO_NULL_Connection_Purging_Strategy:
                            public TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_NULL_Connection_Purging_Strategy (TAO_Resource_Factory* rf);

  /// The destructor
  virtual ~TAO_NULL_Connection_Purging_Strategy ();

protected:
  /// Does nothing.
  virtual int fill_set_i (DESCRIPTOR_SET& sorted_set);

  /// Does nothing.
  virtual void update_item_i (TAO_DESCRIPTOR_INTERFACE* prop,
                              TAO_PURGING_CACHE_ITEM* item);

  /// Does nothing.
  virtual void remove_item_i (TAO_DESCRIPTOR_INTERFACE* prop);

  /// Does nothing.
  virtual void remove_all_i ();

};

#include "ace/post.h"
#endif /* TAO_NULL_PURGING_STRATEGY_H */
