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
    lane_resources_ (0)
{
}

TAO_Default_Thread_Lane_Resources_Manager::~TAO_Default_Thread_Lane_Resources_Manager (void)
{
  delete this->lane_resources_;
}

int
TAO_Default_Thread_Lane_Resources_Manager::initialize (TAO_ORB_Core &orb_core)
{
  int result =
    this->TAO_Thread_Lane_Resources_Manager::initialize (orb_core);

  if (result != 0)
    return result;

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

int
TAO_Default_Thread_Lane_Resources_Manager::shutdown_all_reactors (CORBA_Environment &)
{
  TAO_Leader_Follower &leader_follower =
    this->lane_resources_->leader_follower ();

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    leader_follower.lock (),
                    -1);

  // Wakeup all the threads waiting blocked in the event loop, this
  // does not guarantee that they will all go away, but reduces the
  // load on the POA....
  ACE_Reactor *reactor =
    leader_follower.reactor ();

  reactor->wakeup_all_threads ();

  // If there are some client threads running we have to wait until
  // they finish, when the last one does it will shutdown the reactor
  // for us.  Meanwhile no new requests will be accepted because the
  // POA will not process them.
  if (!leader_follower.has_clients ())
    {
      // Wake up all waiting threads in the reactor.
      reactor->end_reactor_event_loop ();
    }

  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_Default_Thread_Lane_Resources_Manager,
                       ACE_TEXT ("Default_Thread_Lane_Resources_Manager"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Thread_Lane_Resources_Manager),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Thread_Lane_Resources_Manager)
