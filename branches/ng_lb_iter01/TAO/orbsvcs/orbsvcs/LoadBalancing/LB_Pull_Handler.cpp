// -*- C++ -*-

#include "LB_Pull_Handler.h"

#include "tao/debug.h"

ACE_RCSID (LoadBalancing,
           LB_Pull_Handler,
           "$Id$")

TAO_LB_Pull_Handler::TAO_LB_Pull_Handler (
  TAO_LB_Location_Map &location_map)
  : location_map_ (location_map)
{
}

int
TAO_LB_Pull_Handler::handle_timeout (
  const ACE_Time_Value & /* current_time */,
  const void * /* arg */)
{
  TAO_LB_Location_Map::iterator begin =
    this->location_map_.begin ();

  TAO_LB_Location_Map::iterator end =
    this->location_map_.end ();

  // Iterate over all registered load monitors.
  //
  // @todo This could be potentially very slow.  Improve concurrent
  //       operation at some point in the near future.
  for (TAO_LB_Location_Map::iterator i = begin;
       i != end;
       ++i)
    {
      TAO_LB_Location_Map_Entry *location = (*i).int_id_;

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          location->load_list =
            location->load_monitor->current_load (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Catch the exception and ignore it.

          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "(%P|%t) Load monitoring exception");
        }
      ACE_ENDTRY;
    }

  return 0;
}
