// $Id$

#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/Leader_Follower.h"
#include "tao/LF_Strategy.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, Thread_Lane_Resources_Manager, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/Thread_Lane_Resources_Manager.i"
#endif /* ! __ACE_INLINE__ */

TAO_Thread_Lane_Resources_Manager::TAO_Thread_Lane_Resources_Manager (TAO_ORB_Core &orb_core)
  : orb_core_ (&orb_core),
    lf_strategy_ (0)
{
  this->lf_strategy_ =
    this->orb_core_->resource_factory ()->create_lf_strategy ();
}

TAO_Thread_Lane_Resources_Manager::~TAO_Thread_Lane_Resources_Manager (void)
{
  delete this->lf_strategy_;
}

TAO_LF_Strategy &
TAO_Thread_Lane_Resources_Manager::lf_strategy (void)
{
  return *this->lf_strategy_;
}

TAO_Thread_Lane_Resources_Manager_Factory::~TAO_Thread_Lane_Resources_Manager_Factory (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
