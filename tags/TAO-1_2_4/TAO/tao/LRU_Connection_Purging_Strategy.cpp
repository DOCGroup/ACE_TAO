// $Id$

#include "LRU_Connection_Purging_Strategy.h"
#include "Transport.h"

ACE_RCSID(tao, LRU_Connection_Purging_Strategy, "$Id$")

TAO_LRU_Connection_Purging_Strategy::TAO_LRU_Connection_Purging_Strategy (
                                                           int cache_maximum)
 : TAO_Connection_Purging_Strategy (cache_maximum),
   order_ (0)
{
}


TAO_LRU_Connection_Purging_Strategy::~TAO_LRU_Connection_Purging_Strategy (void)
{
}


void
TAO_LRU_Connection_Purging_Strategy::update_item (TAO_Transport* transport)
{
  transport->purging_order (this->order_++);
}

