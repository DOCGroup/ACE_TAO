// $Id$

#include "FIFO_Connection_Purging_Strategy.h"

ACE_RCSID(tao, FIFO_Connection_Purging_Strategy, "$Id$")

TAO_FIFO_Connection_Purging_Strategy::TAO_FIFO_Connection_Purging_Strategy (
                                               TAO_Resource_Factory* rf)
 : TAO_ULong_Connection_Purging_Strategy (rf),
   order_ (0)
{
}


TAO_FIFO_Connection_Purging_Strategy::~TAO_FIFO_Connection_Purging_Strategy ()
{
}


void
TAO_FIFO_Connection_Purging_Strategy::update_item_i (
                                       TAO_DESCRIPTOR_INTERFACE* prop,
                                       TAO_PURGING_CACHE_ITEM* item)
{
  ACE_UNUSED_ARG (item);
  TAO_Cache_ExtId ext_id (prop);
  ATTRIBUTE_TYPE int_id (this->order_);
  if (this->tracking_map ().find (ext_id, int_id) != 0)
    {
      this->tracking_map ().bind(ext_id, int_id);
      this->order_++;
    }
}


