// $Id$

#include "tao/Thread_Lane_Resources.h"

ACE_RCSID(tao, Thread_Lane_Resources, "$Id$")

#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/Thread_Lane_Resources.i"
#endif /* ! __ACE_INLINE__ */

TAO_Thread_Lane_Resources::TAO_Thread_Lane_Resources (TAO_ORB_Core &orb_core)
  : orb_core_ (orb_core),
    acceptor_registry_ (0),
    transport_cache_ (0),
    leader_follower_ (0)
{
  // Create the transport cache.
  ACE_NEW (this->transport_cache_,
           TAO_Transport_Cache_Manager (orb_core));

}

TAO_Thread_Lane_Resources::~TAO_Thread_Lane_Resources (void)
{
  delete this->transport_cache_;
  delete this->leader_follower_;
}

TAO_Transport_Cache_Manager &
TAO_Thread_Lane_Resources::transport_cache (void)
{
  return *this->transport_cache_;
}

int
TAO_Thread_Lane_Resources::has_acceptor_registry_been_created (void) const
{
  return this->acceptor_registry_ != 0;
}

TAO_Acceptor_Registry &
TAO_Thread_Lane_Resources::acceptor_registry (void)
{
  // Double check.
  if (this->acceptor_registry_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, *this->acceptor_registry_);
      if (this->acceptor_registry_ == 0)
        {
          TAO_Resource_Factory &resource_factory =
            *this->orb_core_.resource_factory ();

          this->acceptor_registry_ =
            resource_factory.get_acceptor_registry ();
        }
    }

  return *this->acceptor_registry_;
}

TAO_Leader_Follower &
TAO_Thread_Lane_Resources::leader_follower (void)
{
  // Double check.
  if (this->leader_follower_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, *this->leader_follower_);
      if (this->leader_follower_ == 0)
        {
          ACE_NEW_RETURN (this->leader_follower_,
                          TAO_Leader_Follower (&this->orb_core_),
                          *this->leader_follower_);
        }
    }

  return *this->leader_follower_;
}

int
TAO_Thread_Lane_Resources::open_acceptor_registry (CORBA::Environment &ACE_TRY_ENV)
{
  /// Access the acceptor registry.
  TAO_Acceptor_Registry &ar =
    this->acceptor_registry ();

  int result =
    ar.open (&this->orb_core_,
             this->leader_follower ().reactor (),
             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return result;
}

void
TAO_Thread_Lane_Resources::finalize (void)
{
  // Ask the registry to close all registered acceptors.
  if (this->acceptor_registry_ != 0)
    {
      this->acceptor_registry_->close_all ();
      delete this->acceptor_registry_;
    }

  // Set of file descriptors corresponding to open connections.  This
  // handle set is used to explicitly deregister the connection event
  // handlers from the Reactor.  This is particularly important for
  // dynamically loaded ORBs where an application level reactor, such
  // as the Singleton reactor, is used instead of an ORB created one.
  ACE_Handle_Set handle_set;
  TAO_EventHandlerSet unregistered;

  // Close the transport cache and return the handle set that needs
  // to be de-registered from the reactor.
  this->transport_cache_->close (handle_set, unregistered);

  // Shutdown all open connections that are registered with the ORB
  // Core.  Note that the ACE_Event_Handler::DONT_CALL mask is NOT
  // used here since the reactor should invoke each handle's
  // corresponding ACE_Event_Handler::handle_close() method to ensure
  // that the connection is shutdown gracefully prior to destroying
  // the ORB Core.
  if (handle_set.num_set () > 0)
    (void) this->leader_follower ().reactor ()->remove_handler (handle_set,
                                                                ACE_Event_Handler::ALL_EVENTS_MASK);
  if (!unregistered.is_empty ())
    {
      ACE_Event_Handler** eh;
      for (TAO_EventHandlerSetIterator iter(unregistered);
           iter.next (eh);
           iter.advance())
        {
          (*eh)->handle_close (ACE_INVALID_HANDLE,
                               ACE_Event_Handler::ALL_EVENTS_MASK);
        }
    }
}

// ****************************************************************

TAO_Thread_Lane_Resources_Manager::~TAO_Thread_Lane_Resources_Manager (void)
{
}

// ****************************************************************

TAO_Default_Thread_Lane_Resources_Manager::TAO_Default_Thread_Lane_Resources_Manager (void)
  : open_called_ (0),
    lane_resources_ (0),
    orb_core_ (0)
{
}

TAO_Default_Thread_Lane_Resources_Manager::~TAO_Default_Thread_Lane_Resources_Manager (void)
{
  delete this->lane_resources_;
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
TAO_Default_Thread_Lane_Resources_Manager::open (CORBA_Environment &ACE_TRY_ENV)
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
}

TAO_Thread_Lane_Resources &
TAO_Default_Thread_Lane_Resources_Manager::lane_resources (void)
{
  return *this->lane_resources_;
}

// ****************************************************************

TAO_RT_Thread_Lane_Resources_Manager::TAO_RT_Thread_Lane_Resources_Manager (void)
  : open_called_ (0),
    default_lane_resources_ (0),
    orb_core_ (0)
{
}

TAO_RT_Thread_Lane_Resources_Manager::~TAO_RT_Thread_Lane_Resources_Manager (void)
{
  delete this->default_lane_resources_;
}

int
TAO_RT_Thread_Lane_Resources_Manager::initialize (TAO_ORB_Core &orb_core)
{
  this->orb_core_ =
    &orb_core;

  ACE_NEW_RETURN (this->default_lane_resources_,
                  TAO_Thread_Lane_Resources (orb_core),
                  -1);

  return 0;
}

int
TAO_RT_Thread_Lane_Resources_Manager::open (CORBA_Environment &ACE_TRY_ENV)
{
  // Double check pattern
  if (this->open_called_ == 1)
    return 1;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, tao_mon, this->open_lock_, -1);

  if (this->open_called_ == 1)
    return 1;

  TAO_Acceptor_Registry &ar =
    this->default_lane_resources_->acceptor_registry ();
  // get a reference to the acceptor_registry!

  int ret =
    ar.open (this->orb_core_,
             this->default_lane_resources_->leader_follower ().reactor (),
             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

  this->open_called_ = 1;

  return 0;
}

void
TAO_RT_Thread_Lane_Resources_Manager::finalize (void)
{
  this->default_lane_resources_->finalize ();
}

TAO_Thread_Lane_Resources &
TAO_RT_Thread_Lane_Resources_Manager::lane_resources (void)
{
  /// Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources &tss =
    this->orb_core_->get_tss_resources ();

  /// Get the lane for this thread.
  TAO_Thread_Lane *lane =
    (TAO_Thread_Lane *) tss.lane_;

  if (lane)
    return lane->resources ();
  else
    return *this->default_lane_resources_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Thread_Lane_Resources_Manager,
                       ACE_TEXT ("Default_Thread_Lane_Resources_Manager"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Thread_Lane_Resources_Manager),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Thread_Lane_Resources_Manager)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager>

#endif
