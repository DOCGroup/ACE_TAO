// $Id$

#include "tao/RTCORBA/RT_Thread_Lane_Resources_Manager.h"

ACE_RCSID(RTCORBA, RT_Thread_Lane_Resources_Manager, "$Id$")

#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/RTCORBA/RT_Thread_Lane_Resources_Manager.i"
#endif /* ! __ACE_INLINE__ */

TAO_RT_Thread_Lane_Resources_Manager::TAO_RT_Thread_Lane_Resources_Manager (TAO_ORB_Core &orb_core)
  : TAO_Thread_Lane_Resources_Manager (orb_core),
    open_called_ (0),
    default_lane_resources_ (0),
    tp_manager_ (0)
{
  // Create the default resources.
  ACE_NEW (this->default_lane_resources_,
           TAO_Thread_Lane_Resources (orb_core));

  // Create the thread-pool manager.
  ACE_NEW (this->tp_manager_,
           TAO_Thread_Pool_Manager (orb_core));

}

TAO_RT_Thread_Lane_Resources_Manager::~TAO_RT_Thread_Lane_Resources_Manager (void)
{
  // Delete the default resources.
  delete this->default_lane_resources_;

  // Delete the thread-pool manager.
  delete this->tp_manager_;
}

int
TAO_RT_Thread_Lane_Resources_Manager::open_default_resources (CORBA_Environment &ACE_TRY_ENV)
{
  // Check if we have been already opened.
  if (this->open_called_ == 1)
    return 1;

  // Double check pattern
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, tao_mon, this->open_lock_, -1);

  if (this->open_called_ == 1)
    return 1;

  // Get a reference to the acceptor_registry.
  TAO_Acceptor_Registry &ar =
    this->default_lane_resources_->acceptor_registry ();

  // Open it.
  int ret =
    ar.open (this->orb_core_,
             this->default_lane_resources_->leader_follower ().reactor (),
             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

  // Remember that the acceptor registry has already been opened.
  this->open_called_ = 1;

  return 0;
}

void
TAO_RT_Thread_Lane_Resources_Manager::finalize (void)
{
  // Finalize default resources.
  this->default_lane_resources_->finalize ();

  // Finalize resources managed by the thread-pool manager.
  this->tp_manager_->finalize ();
}

void
TAO_RT_Thread_Lane_Resources_Manager::shutdown_reactor (void)
{
  // Shutdown default reactors.
  this->default_lane_resources_->shutdown_reactor ();

  // Shutdown reactors managed by the thread-pool manager.
  this->tp_manager_->shutdown_reactor ();
}

TAO_Thread_Lane_Resources &
TAO_RT_Thread_Lane_Resources_Manager::lane_resources (void)
{
  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources &tss =
    *this->orb_core_->get_tss_resources ();

  // Get the lane for this thread.
  TAO_Thread_Lane *lane =
    (TAO_Thread_Lane *) tss.lane_;

  // If we have a valid lane, use that lane's resources.
  if (lane)
    return lane->resources ();
  else
    // Otherwise, return the default resources.
    return *this->default_lane_resources_;
}

TAO_Thread_Lane_Resources &
TAO_RT_Thread_Lane_Resources_Manager::default_lane_resources (void)
{
  return *this->default_lane_resources_;
}

TAO_Thread_Pool_Manager &
TAO_RT_Thread_Lane_Resources_Manager::tp_manager (void)
{
  return *this->tp_manager_;
}

TAO_Thread_Lane_Resources_Manager *
TAO_RT_Thread_Lane_Resources_Manager_Factory::create_thread_lane_resources_manager (TAO_ORB_Core &core)
{
  TAO_Thread_Lane_Resources_Manager *manager = 0;

  /// Create the RT Thread Lane Resources Manager.
  ACE_NEW_RETURN (manager,
                  TAO_RT_Thread_Lane_Resources_Manager (core),
                  0);

  return manager;
}

ACE_STATIC_SVC_DEFINE (TAO_RT_Thread_Lane_Resources_Manager_Factory,
                       ACE_TEXT ("RT_Thread_Lane_Resources_Manager_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Thread_Lane_Resources_Manager_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_RT_Thread_Lane_Resources_Manager_Factory)
