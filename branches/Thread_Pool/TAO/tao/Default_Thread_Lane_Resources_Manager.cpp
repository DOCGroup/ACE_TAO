// $Id$

#include "tao/Default_Thread_Lane_Resources_Manager.h"

ACE_RCSID(tao, Default_Thread_Lane_Resources_Manager, "$Id$")

#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/Default_Thread_Lane_Resources_Manager.i"
#endif /* ! __ACE_INLINE__ */

TAO_Default_Thread_Lane_Resources_Manager::TAO_Default_Thread_Lane_Resources_Manager (void)
  : open_called_ (0),
    lane_resources_ (0),
    orb_core_ (0)
{
}

TAO_Default_Thread_Lane_Resources_Manager::~TAO_Default_Thread_Lane_Resources_Manager (void)
{
}

int
TAO_Default_Thread_Lane_Resources_Manager::initialize (TAO_ORB_Core &orb_core)
{
  this->orb_core_ =
    &orb_core;

  ACE_NEW_RETURN (this->lane_resources_,
                  TAO_Thread_Lane_Resources (orb_core),
                  -1);

  return 0;
}

int
TAO_Default_Thread_Lane_Resources_Manager::open_default_resources (CORBA_Environment &ACE_TRY_ENV)
{
  // Double check pattern
  if (this->open_called_ == 1)
    return 1;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, tao_mon, this->open_lock_, -1);

  if (this->open_called_ == 1)
    return 1;

  TAO_Acceptor_Registry &ar =
    this->lane_resources_->acceptor_registry ();
  // get a reference to the acceptor_registry!

  int ret =
    ar.open (this->orb_core_,
             this->lane_resources_->leader_follower ().reactor (),
             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

  this->open_called_ = 1;

  return 0;
}

void
TAO_Default_Thread_Lane_Resources_Manager::finalize (void)
{
  this->lane_resources_->finalize ();
  delete this->lane_resources_;
  this->lane_resources_ = 0;
}

TAO_Thread_Lane_Resources &
TAO_Default_Thread_Lane_Resources_Manager::lane_resources (void)
{
  return *this->lane_resources_;
}

TAO_Thread_Lane_Resources &
TAO_Default_Thread_Lane_Resources_Manager::default_lane_resources (void)
{
  return *this->lane_resources_;
}

ACE_STATIC_SVC_DEFINE (TAO_Default_Thread_Lane_Resources_Manager,
                       ACE_TEXT ("Default_Thread_Lane_Resources_Manager"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Thread_Lane_Resources_Manager),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Thread_Lane_Resources_Manager)
