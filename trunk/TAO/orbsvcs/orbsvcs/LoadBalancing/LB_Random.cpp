// -*- C++ -*-

#include "LB_Random.h"

ACE_RCSID (LoadBalancing,
           LB_Random,
           "$Id$")

TAO_LB_Random::TAO_LB_Random (void)
{
  // Seed the random number generator with the current time.
  ACE_OS::srand (ACE_static_cast (unsigned int, ACE_OS::time ()));
}

char *
TAO_LB_Random::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Random");
}
    
CosLoadBalancing::Properties *
TAO_LB_Random::get_properties (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_LB_Random::push_loads (
    const PortableGroup::Location & /* the_location */,
    const CosLoadBalancing::LoadList & /* loads */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::StrategyNotAdaptive))
{
  ACE_THROW (CosLoadBalancing::StrategyNotAdaptive ());
}
    
CORBA::Object_ptr
TAO_LB_Random::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  const CORBA::ULong len = locations->length ();
  if (len == 0)
    ACE_THROW_RETURN (CORBA::TRANSIENT (),
                      CORBA::Object::_nil ());

  // Pick a random location in the sequence using the higher order
  // bits (zero based indexing).
  //
  // See Numerical Recipes in C: The Art of Scientific Computing
  // (William  H.  Press, Brian P. Flannery, Saul A. Teukolsky,
  // William T.  Vetterling;  New  York:  Cambridge University Press,
  // 1992 (2nd ed., p. 277)) for details on why using the lower order
  // bits, as in (rand() % 10), is bad.  In particular, the lower
  // order bits in older rand() implementations may not be as random
  // as the higher order bits.
  //
  // @todo It would be better to use the random() function since it
  //       is supposed to have a larger period than rand(), in
  //       addition to the fact that the lower order bits should be as
  //       random as the higher order bits.
  const CORBA::ULong i =
    ACE_static_cast (CORBA::ULong,
                     (len * ACE_OS::rand () / (RAND_MAX + 1.0)));

  ACE_ASSERT (i < len);

  return load_manager->get_member_ref (object_group,
                                       locations[i]
                                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_Random::analyze_loads (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    CosLoadBalancing::LoadManager_ptr /* load_manager */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
