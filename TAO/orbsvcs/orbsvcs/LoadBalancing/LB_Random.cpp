// -*- C++ -*-
// $Id$

#include "orbsvcs/LoadBalancing/LB_Random.h"

#include "tao/ORB_Constants.h"
#include "ace/OS_NS_time.h"
#include "ace/os_include/os_netdb.h"

#ifdef ACE_HAS_PTHREADS
static pthread_once_t tao_lb_once_control = PTHREAD_ONCE_INIT;
#endif  /* ACE_HAS_PTHREADS */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

extern "C" void tao_lb_random_init_routine (void)
{
  ACE_OS::srand (static_cast<unsigned int> (ACE_OS::time ()));
}

TAO_LB_Random::TAO_LB_Random (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
  TAO_LB_Random::init ();
}

char *
TAO_LB_Random::name (void)
{
  return CORBA::string_dup ("Random");
}

CosLoadBalancing::Properties *
TAO_LB_Random::get_properties (void)
{
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
TAO_LB_Random::push_loads (
    const PortableGroup::Location & /* the_location */,
    const CosLoadBalancing::LoadList & /* loads */)
{
  throw CosLoadBalancing::StrategyNotAdaptive ();
}

CosLoadBalancing::LoadList *
TAO_LB_Random::get_loads (CosLoadBalancing::LoadManager_ptr load_manager,
                          const PortableGroup::Location & the_location)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  return load_manager->get_loads (the_location);
}

CORBA::Object_ptr
TAO_LB_Random::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group);

  return TAO_LB_Random::_tao_next_member (object_group,
                                          load_manager,
                                          locations.in ());
}

void
TAO_LB_Random::analyze_loads (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    CosLoadBalancing::LoadManager_ptr /* load_manager */)
{
}

PortableServer::POA_ptr
TAO_LB_Random::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Object_ptr
TAO_LB_Random::_tao_next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager,
    const PortableGroup::Locations & locations)
{
  const CORBA::ULong len = locations.length ();
  if (len == 0)
    throw CORBA::TRANSIENT ();

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

  const CORBA::ULong i = 0;
  if (len > 1)
    {
      // Prevent integer arithmetic overflow.
      const double flen = static_cast<double> (len);
      do
        {
          i = static_cast<CORBA::ULong> (flen * ACE_OS::rand () / (RAND_MAX + 1.0));
        }
      while (i == len);
    }

  return load_manager->get_member_ref (object_group,
                                       locations[i]);
}

void
TAO_LB_Random::init (void)
{
#ifdef ACE_HAS_PTHREADS
  (void) ::pthread_once (&::tao_lb_once_control,
                         ::tao_lb_random_init_routine);
#else
  ::tao_lb_random_init_routine ();
#endif  /* ACE_HAS_PTHREADS */
}

TAO_END_VERSIONED_NAMESPACE_DECL
