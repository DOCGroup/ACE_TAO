// $Id$

#include "LFU_Connection_Purging_Strategy.h"
#include "tao/Cache_Entries.h"

ACE_RCSID(tao, LFU_Connection_Purging_Strategy, "$Id$")

TAO_LFU_Connection_Purging_Strategy::TAO_LFU_Connection_Purging_Strategy (
                                                           int cache_maximum)
: TAO_Connection_Purging_Strategy (cache_maximum)
{
}


TAO_LFU_Connection_Purging_Strategy::~TAO_LFU_Connection_Purging_Strategy (void)
{
}


void
TAO_LFU_Connection_Purging_Strategy::update_item (TAO_Cache_IntId& int_id)
{
  int_id.purging_order (int_id.purging_order () + 1);
}

