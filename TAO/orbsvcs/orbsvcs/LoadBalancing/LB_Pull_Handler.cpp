// -*- C++ -*-

#include "LB_Pull_Handler.h"

ACE_RCSID (LoadBalancing,
           LB_Pull_Handler,
           "$Id$")

TAO_LB_Pull_Handler::TAO_LB_Pull_Handler (
  TAO_LB_ObjectGroup_Map &object_group_map)
  : object_group_map_ (object_group_map)
{
}

int
TAO_LB_Pull_Handler::handle_timeout (
  const ACE_Time_Value & /* current_time */,
  const void * /* arg */)
{
  TAO_LB_ObjectGroup_Map::iterator begin =
    this->object_group_map_.begin ();

  TAO_LB_ObjectGroup_Map::iterator begin =
    this->object_group_map_.end ();

  // Iterate over all registered object groups.
  //
  // @todo This could be potentially very slow.  Improve concurrent
  //       operation at some point in the near future.
  for (TAO_LB_ObjectGroup_Map::iterator i = begin;
       i != end;
       ++i
  {
    TAO_LB_ObjectGroup_Map_Entry *object_group = i->ext_id_;

    
  }


  return 0;
}
