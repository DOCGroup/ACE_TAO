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

TAO_Default_Thread_Lane_Resources_Manager::TAO_Default_Thread_Lane_Resources_Manager (TAO_ORB_Core &orb_core)
  : TAO_Thread_Lane_Resources_Manager (orb_core),
    open_called_ (0),
    lane_resources_ (0)
{
  // Create the default resources.
  ACE_NEW (this->lane_resources_,
           TAO_Thread_Lane_Resources (orb_core));
}

TAO_Default_Thread_Lane_Resources_Manager::~TAO_Default_Thread_Lane_Resources_Manager (void)
{
  // Delete the default resources.
  delete this->lane_resources_;
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

  // Get a reference to the acceptor_registry.
  TAO_Acceptor_Registry &ar =
    this->lane_resources_->acceptor_registry ();

  // Open it.
  int ret =
    ar.open (this->orb_core_,
             this->lane_resources_->leader_follower ().reactor (),
             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

  // Remember that the acceptor registry has already been opened.
  this->open_called_ = 1;

  return 0;
}

void
TAO_Default_Thread_Lane_Resources_Manager::finalize (void)
{
  // Finalize default resources.
  this->lane_resources_->finalize ();
}

TAO_Thread_Lane_Resources &
TAO_Default_Thread_Lane_Resources_Manager::lane_resources (void)
{
  return *this->lane_resources_;
}

TAO_Thread_Lane_Resources &
TAO_Default_Thread_Lane_Resources_Manager::default_lane_resources (void)
{
  return this->lane_resources ();
}

void
TAO_Default_Thread_Lane_Resources_Manager::shutdown_reactor (void)
{
  this->lane_resources_->shutdown_reactor ();
}

TAO_Thread_Lane_Resources_Manager *
TAO_Default_Thread_Lane_Resources_Manager_Factory::create_thread_lane_resources_manager (TAO_ORB_Core &core)
{
  TAO_Thread_Lane_Resources_Manager *manager = 0;

  /// Create the Default Thread Lane Resources Manager.
  ACE_NEW_RETURN (manager,
                  TAO_Default_Thread_Lane_Resources_Manager (core),
                  0);

  return manager;
}

ACE_STATIC_SVC_DEFINE (TAO_Default_Thread_Lane_Resources_Manager_Factory,
                       ACE_TEXT ("Default_Thread_Lane_Resources_Manager_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Thread_Lane_Resources_Manager_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Thread_Lane_Resources_Manager_Factory)
