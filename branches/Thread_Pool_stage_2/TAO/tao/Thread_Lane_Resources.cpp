// $Id$

#include "tao/Thread_Lane_Resources.h"

ACE_RCSID(tao, Thread_Lane_Resources, "$Id$")

#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Leader_Follower.h"
#include "ace/Reactor.h"

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
          // Get the resource factory.
          TAO_Resource_Factory &resource_factory =
            *this->orb_core_.resource_factory ();

          // Ask it to create a new acceptor registry.
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
          // Create a new Leader Follower object.
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

  // Open it.
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

  delete this->transport_cache_;
  delete this->leader_follower_;
}
