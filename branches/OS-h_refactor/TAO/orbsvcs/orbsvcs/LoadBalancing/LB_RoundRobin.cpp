// -*- C++ -*-

#include "LB_RoundRobin.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           LB_RoundRobin,
           "$Id$")


TAO_LB_RoundRobin::TAO_LB_RoundRobin (void)
  //  : lock_ (),
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
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::StrategyNotAdaptive))
{
  ACE_THROW (CosLoadBalancing::StrategyNotAdaptive ());
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
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CORBA::Object::_nil ());
}
