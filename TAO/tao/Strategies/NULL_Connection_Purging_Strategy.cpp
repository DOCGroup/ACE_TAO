// $Id$

#include "NULL_Connection_Purging_Strategy.h"

ACE_RCSID(tao, NULL_Connection_Purging_Strategy, "$Id$")

TAO_NULL_Connection_Purging_Strategy::TAO_NULL_Connection_Purging_Strategy (
                                               TAO_Resource_Factory* rf)
 : TAO_Connection_Purging_Strategy (rf)
{
}


TAO_NULL_Connection_Purging_Strategy::~TAO_NULL_Connection_Purging_Strategy ()
{
}


int
TAO_NULL_Connection_Purging_Strategy::fill_set_i (DESCRIPTOR_SET& sorted_set)
{
  return 0;
}


void
TAO_NULL_Connection_Purging_Strategy::update_item_i (
                                       TAO_DESCRIPTOR_INTERFACE* prop,
                                       TAO_PURGING_CACHE_ITEM* item)
{
}


void
TAO_NULL_Connection_Purging_Strategy::remove_item_i (
                                       TAO_DESCRIPTOR_INTERFACE* prop)
{
}


void
TAO_NULL_Connection_Purging_Strategy::remove_all_i ()
{
}


