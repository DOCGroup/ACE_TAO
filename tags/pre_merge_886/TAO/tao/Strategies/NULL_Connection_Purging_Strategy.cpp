// $Id$

#include "NULL_Connection_Purging_Strategy.h"
#include "tao/Transport.h"

ACE_RCSID(tao, NULL_Connection_Purging_Strategy, "$Id$")

TAO_NULL_Connection_Purging_Strategy::TAO_NULL_Connection_Purging_Strategy (
                                                           int cache_maximum)
 : TAO_Connection_Purging_Strategy (cache_maximum)
{
}


TAO_NULL_Connection_Purging_Strategy::~TAO_NULL_Connection_Purging_Strategy (void)
{
}


void
TAO_NULL_Connection_Purging_Strategy::update_item (TAO_Transport* transport)
{
  ACE_UNUSED_ARG (transport);
}


int
TAO_NULL_Connection_Purging_Strategy::cache_maximum (void) const
{
  return -1;
}
