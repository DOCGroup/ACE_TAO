// $Id$

#include "FIFO_Connection_Purging_Strategy.h"
#include "tao/Cache_Entries.h"

ACE_RCSID(tao, FIFO_Connection_Purging_Strategy, "$Id$")

TAO_FIFO_Connection_Purging_Strategy::TAO_FIFO_Connection_Purging_Strategy (
                                                            int cache_maximum)
 : TAO_Connection_Purging_Strategy (cache_maximum),
   order_ (0)
{
}


TAO_FIFO_Connection_Purging_Strategy::~TAO_FIFO_Connection_Purging_Strategy (void)
{
}


void
TAO_FIFO_Connection_Purging_Strategy::update_item (TAO_Cache_IntId& int_id)
{
  // FIFO, so only update the purging order if this
  // item has not been cached yet.
  if (int_id.purging_order () == 0)
    {
      int_id.purging_order (++(this->order_));
    }
}


