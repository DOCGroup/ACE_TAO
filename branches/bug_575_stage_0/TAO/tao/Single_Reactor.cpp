// $Id$

#include "tao/Single_Reactor.h"
#include "tao/ORB_Core.h"
#include "tao/Resource_Factory.h"
#include "tao/Leader_Follower.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "tao/Single_Reactor.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Single_Reactor, "$Id$")

TAO_Single_Reactor::~TAO_Single_Reactor (void)
{
  delete this->leader_follower_;
}

void
TAO_Single_Reactor::open (TAO_ORB_Core *orb_core)
{
  this->TAO_Reactor_Registry::open (orb_core);
  ACE_NEW (this->leader_follower_,
           TAO_Leader_Follower (this->orb_core ()));
}

ACE_Reactor *
TAO_Single_Reactor::reactor (void)
{
  return this->leader_follower_->reactor ();
}

ACE_Reactor *
TAO_Single_Reactor::reactor (TAO_Acceptor *)
{
  return this->reactor ();
}

TAO_Leader_Follower &
TAO_Single_Reactor::leader_follower (void)
{
  return *this->leader_follower_;
}

TAO_Leader_Follower &
TAO_Single_Reactor::leader_follower (TAO_Acceptor *)
{
  return *this->leader_follower_;
}

void
TAO_Single_Reactor::destroy_tss_cookie (void*)
{
}

int
TAO_Single_Reactor::shutdown_all (void)
{
  TAO_Leader_Follower &leader_follower =
    this->leader_follower ();

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    leader_follower.lock (),
                    -1);

  // Wakeup all the threads waiting blocked in the event loop, this
  // does not guarantee that they will all go away, but reduces the
  // load on the POA....
  ACE_Reactor *reactor =
    this->reactor ();

  reactor->wakeup_all_threads ();

  // If there are some client threads running we have to wait until
  // they finish, when the last one does it will shutdown the
  // reactor for us.  Meanwhile no new requests will be accepted
  // because the POA will not process them.

  if (!leader_follower.has_clients ())
    {
      // Wake up all waiting threads in the reactor.
      reactor->end_reactor_event_loop ();
    }

  return 0;
}
