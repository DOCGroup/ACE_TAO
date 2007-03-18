// -*- C++ -*-

#include "orbsvcs/LoadBalancing/LB_RoundRobin.h"

#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (LoadBalancing,
           LB_RoundRobin,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_RoundRobin::TAO_LB_RoundRobin (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    lock_ (),
    location_index_map_ (TAO_PG_MAX_OBJECT_GROUPS)
{
}

TAO_LB_RoundRobin::~TAO_LB_RoundRobin (void)
{
}

char *
TAO_LB_RoundRobin::name (void)
{
  return CORBA::string_dup ("RoundRobin");
}

CosLoadBalancing::Properties *
TAO_LB_RoundRobin::get_properties (void)
{
  // There are no RoundRobin properties.  Return an empty property
  // list.

  CosLoadBalancing::Properties * props = 0;
  ACE_NEW_THROW_EX (props,
                    CosLoadBalancing::Properties,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return props;
}

void
TAO_LB_RoundRobin::push_loads (
    const PortableGroup::Location & /* the_location */,
    const CosLoadBalancing::LoadList & /* loads */)
{
  throw CosLoadBalancing::StrategyNotAdaptive ();
}

CosLoadBalancing::LoadList *
TAO_LB_RoundRobin::get_loads (CosLoadBalancing::LoadManager_ptr load_manager,
                              const PortableGroup::Location & the_location)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  return load_manager->get_loads (the_location);
}

CORBA::Object_ptr
TAO_LB_RoundRobin::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  const PortableGroup::ObjectGroupId id =
    load_manager->get_object_group_id (object_group);

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    monitor,
                    this->lock_,
                    CORBA::Object::_nil ());

  // Since this is "built-in" strategy, the LoadManager is collocated.
  // There is no need to release the lock during the following
  // invocation.
  //
  // There is a race condition here.  The
  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group);

  const CORBA::ULong len = locations->length ();

  if (len == 0)
    throw CORBA::TRANSIENT ();

  TAO_LB_Location_Index_Map::ENTRY * entry;
  if (this->location_index_map_.find (id, entry) == 0)
    {
      CORBA::ULong & i = entry->int_id_;

      if (len <= i)
        i = 0;  // Reset, i.e. wrap around

      // No need to release the lock since the LoadManager is
      // collocated.
      CORBA::Object_var member =
        load_manager->get_member_ref (object_group,
                                      locations[i]);

      // Increment index to point to next location.
      i++;

      return member._retn ();
    }

  // The first time through this method.  Set up for the next time
  // around, and return the object reference residing at the first
  // location in the "locations of members" sequence.

  // Note that it is safe to set the next_index below to 1 even if the
  // length of the sequence is 1 since the above code handles the
  // boundary case correctly by wrapping around.

  const CORBA::ULong index = 0;
  if (this->location_index_map_.bind (id, index + 1) != 0)
    throw CORBA::INTERNAL ();

  return load_manager->get_member_ref (object_group,
                                       locations[index]);
}

void
TAO_LB_RoundRobin::analyze_loads (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    CosLoadBalancing::LoadManager_ptr /* load_manager */)
{
}

PortableServer::POA_ptr
TAO_LB_RoundRobin::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
