// $Id$

#include "LRU_Connection_Purging_Strategy.h"

ACE_RCSID(tao, LRU_Connection_Purging_Strategy, "$Id$")

TAO_LRU_Connection_Purging_Strategy::TAO_LRU_Connection_Purging_Strategy (
                                               TAO_Resource_Factory* rf)
 : TAO_ULong_Connection_Purging_Strategy (rf),
   order_ (0)
{
}


TAO_LRU_Connection_Purging_Strategy::~TAO_LRU_Connection_Purging_Strategy ()
{
}


void
TAO_LRU_Connection_Purging_Strategy::update_item_i (
                                       TAO_DESCRIPTOR_INTERFACE *prop,
                                       TAO_PURGING_CACHE_ITEM* /*item */)
{
  TAO_Cache_ExtId ext_id (prop);
  ATTRIBUTE_TYPE int_id (this->order_++);
  this->tracking_map ().rebind(ext_id, int_id);
}
