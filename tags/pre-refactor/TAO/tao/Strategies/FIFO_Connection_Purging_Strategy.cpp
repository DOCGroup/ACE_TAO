// $Id$

#include "FIFO_Connection_Purging_Strategy.h"
#include "tao/Transport.h"

ACE_RCSID(tao, FIFO_Connection_Purging_Strategy, "$Id$")

TAO_FIFO_Connection_Purging_Strategy::TAO_FIFO_Connection_Purging_Strategy (
                                                            int cache_maximum)
 : TAO_Connection_Purging_Strategy (cache_maximum),
   // Initialized to 1 to insure that the transports purging_order
   // is only 0 upon initialization.
   order_ (1)
{
}


TAO_FIFO_Connection_Purging_Strategy::~TAO_FIFO_Connection_Purging_Strategy (void)
{
}


void
TAO_FIFO_Connection_Purging_Strategy::update_item (TAO_Transport* transport)
{
  // FIFO, so only update the purging order if this
  // item has not been cached yet.
  if (transport->purging_order () == 0)
    {
      transport->purging_order (this->order_++);
    }
}


