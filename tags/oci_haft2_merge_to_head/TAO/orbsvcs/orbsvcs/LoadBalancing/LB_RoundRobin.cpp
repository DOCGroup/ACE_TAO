// -*- C++ -*-

#include "LB_RoundRobin.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           LB_RoundRobin,
           "$Id$")


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
TAO_LB_RoundRobin::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("RoundRobin");
}

CosLoadBalancing::Properties *
TAO_LB_RoundRobin::get_properties (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // There are no RoundRobin properties.  Return an empty property
  // list.

  CosLoadBalancing::Properties * props = 0;
  ACE_NEW_THROW_EX (props,
                    CosLoadBalancing::Properties,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (props);

  return props;
}

void
TAO_LB_RoundRobin::push_loads (
    const PortableGroup::Location & /* the_location */,
    const CosLoadBalancing::LoadList & /* loads */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::StrategyNotAdaptive))
{
  ACE_THROW (CosLoadBalancing::StrategyNotAdaptive ());
}

CosLoadBalancing::LoadList *
TAO_LB_RoundRobin::get_loads (CosLoadBalancing::LoadManager_ptr load_manager,
                              const PortableGroup::Location & the_location
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LocationNotFound))
{
  if (CORBA::is_nil (load_manager))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  return load_manager->get_loads (the_location
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_LB_RoundRobin::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  if (CORBA::is_nil (load_manager))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), CORBA::Object::_nil ());

  const PortableGroup::ObjectGroupId id =
    load_manager->get_object_group_id (object_group
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

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
    load_manager->locations_of_members (object_group
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  const CORBA::ULong len = locations->length ();

  if (len == 0)
    ACE_THROW_RETURN (CORBA::TRANSIENT (),
                      CORBA::Object::_nil ());

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
                                      locations[i]
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

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
    ACE_THROW_RETURN (CORBA::INTERNAL (), CORBA::Object::_nil ());

  return load_manager->get_member_ref (object_group,
                                       locations[index]
                                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_RoundRobin::analyze_loads (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    CosLoadBalancing::LoadManager_ptr /* load_manager */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

PortableServer::POA_ptr
TAO_LB_RoundRobin::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
