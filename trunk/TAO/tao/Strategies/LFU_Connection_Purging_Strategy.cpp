// $Id$

#include "LFU_Connection_Purging_Strategy.h"

ACE_RCSID(tao, LFU_Connection_Purging_Strategy, "$Id$")

TAO_LFU_Connection_Purging_Strategy::TAO_LFU_Connection_Purging_Strategy (
                                               TAO_Resource_Factory* rf)
 : TAO_ULong_Connection_Purging_Strategy (rf)
{
}


TAO_LFU_Connection_Purging_Strategy::~TAO_LFU_Connection_Purging_Strategy ()
{
}


void
TAO_LFU_Connection_Purging_Strategy::update_item_i (
                                       TAO_DESCRIPTOR_INTERFACE* prop,
                                       TAO_PURGING_CACHE_ITEM* item)
{
  ACE_UNUSED_ARG (item);
  TAO_Cache_ExtId ext_id (prop);
  ATTRIBUTE_TYPE int_id (0);
  if (this->tracking_map ().find (ext_id, int_id) == 0)
    {
      int_id++;
    }
  this->tracking_map ().rebind(ext_id, int_id);
}

