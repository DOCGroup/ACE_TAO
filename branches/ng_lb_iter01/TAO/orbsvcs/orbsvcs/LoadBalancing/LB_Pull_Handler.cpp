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

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, object_group->lock, 0);

    if (entry->replica_infos.is_empty ())
      // @@ What do we do if the set is empty?
      continue;

    TAO_LB_ReplicaInfo_Set::iterator begin =
      object_group->replica_infos.begin ();

    TAO_LB_ReplicaInfo_Set::iterator end =
      object_group->replica_infos.end ();

    // Now iterate over the replica set.

      TAO_LB_ReplicaInfo *replica_info = (*i);

      for (TAO_LB_ReplicaInfo_Set::iterator i = begin;
           i != end;
           ++i)
        {
          LoadBalancing::LoadList_var load =
            (*i)->load_monitor->current_load (ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          // @@ Hardcode one load and don't bother checking the
          // LoadId, for now.  (just to get things going)
          if (d[CORBA::Long (0)].value > load[CORBA::Long (0)].value)
            {
              replica_info = *i;
              d = (*i)->load_monitor->current_load (ACE_TRY_ENV);
              ACE_CHECK_RETURN (CORBA::Object::_nil ());
            }
        }

  }


  return 0;
}
