// $Id$

#include "Reactor_Per_Priority.h"
#include "tao/ORB_Core.h"
#include "tao/Resource_Factory.h"
#include "tao/Leader_Follower.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "Reactor_Per_Priority.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(Strategies, Reactor_Per_Priority, "$Id$")

TAO_Reactor_Per_Priority::~TAO_Reactor_Per_Priority (void)
{
  for (Map_Iterator i = this->map_.begin ();
       i != this->map_.end ();
       ++i)
    {
      delete (*i).int_id_;
    }
}

ACE_Reactor *
TAO_Reactor_Per_Priority::reactor (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  TAO_ORB_Core_TSS_Resources *tss =
    this->orb_core ()->get_tss_resources ();

  TAO_Leader_Follower *leader_follower =
    ACE_static_cast (TAO_Leader_Follower*,
                     tss->reactor_registry_cookie_);

  if (leader_follower != 0)
    return leader_follower->reactor ();

  TAO_Protocols_Hooks *tph =
    this->orb_core ()->get_protocols_hooks (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::Short priority = 0;
  if (tph->get_thread_priority (priority,
                                ACE_TRY_ENV)
      == -1)
    {
      if (TAO_debug_level > 3)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Reactor_Per_Priority::reactor: ")
                    ACE_TEXT (" cannot get priority for this thread\n")));
      return 0;
    }

  leader_follower = this->leader_follower_i (priority);

  return leader_follower->reactor ();
}

ACE_Reactor *
TAO_Reactor_Per_Priority::reactor (TAO_Acceptor *acceptor)
{
  CORBA::Short priority =
    acceptor->priority ();

  TAO_Leader_Follower *leader_follower =
    this->leader_follower_i (priority);

  return leader_follower->reactor ();
}

TAO_Leader_Follower &
TAO_Reactor_Per_Priority::leader_follower (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  TAO_ORB_Core_TSS_Resources *tss =
    this->orb_core ()->get_tss_resources ();

  TAO_Leader_Follower *leader_follower =
    ACE_static_cast (TAO_Leader_Follower*,
                     tss->reactor_registry_cookie_);

  if (leader_follower != 0)
    return *leader_follower;

  TAO_Protocols_Hooks *tph =
    this->orb_core ()->get_protocols_hooks (ACE_TRY_ENV);
  ACE_CHECK_RETURN (*leader_follower);

  CORBA::Short priority = 0;
  if (tph->get_thread_priority (priority,
                                ACE_TRY_ENV)
      == -1)
    return *leader_follower;

  return *this->leader_follower_i (priority);
}

TAO_Leader_Follower &
TAO_Reactor_Per_Priority::leader_follower (TAO_Acceptor *acceptor)
{
  CORBA::Short priority =
    acceptor->priority ();

  return *this->leader_follower_i (priority);
}

void
TAO_Reactor_Per_Priority::destroy_tss_cookie (void*)
{
  // Do nothing, data is destroyed in the map...
}

TAO_Leader_Follower *
TAO_Reactor_Per_Priority::leader_follower_i (CORBA::Short priority)
{
  // If the priority of the current thread is not right we return.
  TAO_Leader_Follower *leader_follower = 0;
  if (this->map_.find (priority, leader_follower) == -1)
    {
      if (TAO_debug_level > 3)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - new priority %d\n"),
                    priority));
      // The priority is new, create an entry in the table.
      ACE_NEW_RETURN (leader_follower,
                      TAO_Leader_Follower (this->orb_core ()),
                      leader_follower);
      this->map_.bind (priority, leader_follower);
    }

  return leader_follower;
}

int
TAO_Reactor_Per_Priority::shutdown_all (void)
{
  for (Map_Iterator i = this->map_.begin ();
       i != this->map_.end ();
       ++i)
    {
      TAO_Leader_Follower &leader_follower =
        *((*i).int_id_);

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        leader_follower.lock (),
                        -1);

      // Wakeup all the threads waiting blocked in the event loop,
      // this does not guarantee that they will all go away, but
      // reduces the load on the POA....
      ACE_Reactor *reactor =
        leader_follower.reactor ();

      reactor->wakeup_all_threads ();

      // If there are some client threads running we have to wait
      // until they finish, when the last one does it will shutdown
      // the reactor for us.  Meanwhile no new requests will be
      // accepted because the POA will not process them.

      if (!leader_follower.has_clients ())
        {
          // Wake up all waiting threads in the reactor.
          reactor->end_reactor_event_loop ();
        }
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Map_Manager<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>;
template class ACE_Map_Iterator<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>;
template class ACE_Map_Iterator_Base<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>;
template class ACE_Map_Reverse_Iterator<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>;
template class ACE_Map_Entry<CORBA::Short,TAO_Leader_Follower*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Map_Manager<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Iterator<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Entry<CORBA::Short,TAO_Leader_Follower*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
