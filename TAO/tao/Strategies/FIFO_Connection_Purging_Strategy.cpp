// $Id$

#include "tao/Strategies/FIFO_Connection_Purging_Strategy.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FIFO_Connection_Purging_Strategy::TAO_FIFO_Connection_Purging_Strategy (
                                                            int cache_maximum)
 : TAO_Connection_Purging_Strategy (cache_maximum),
   // Initialized to 1 to insure that the transports purging_order
   // is only 0 upon initialization.
   order_ (1)
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

TAO_END_VERSIONED_NAMESPACE_DECL
