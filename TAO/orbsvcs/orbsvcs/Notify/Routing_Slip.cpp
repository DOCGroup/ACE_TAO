// $Id$

#include "orbsvcs/Notify/Routing_Slip.h"

#include "orbsvcs/Notify/Delivery_Request.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/Event_Persistence_Strategy.h"
#include "orbsvcs/Notify/Routing_Slip_Persistence_Manager.h"
#include "orbsvcs/Notify/Routing_Slip_Queue.h"
#include "orbsvcs/Notify/Method_Request_Lookup.h"
#include "orbsvcs/Notify/Method_Request_Dispatch.h"

#include "tao/debug.h"
#include "tao/corba.h"

#include "ace/Dynamic_Service.h"
#include "ace/Truncate.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

#define QUEUE_ALLOWED 1

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
///////////////////////
// Routing_Slip Statics

Routing_Slip_Queue Routing_Slip::persistent_queue_(QUEUE_ALLOWED);

TAO_SYNCH_MUTEX Routing_Slip::sequence_lock_;
int Routing_Slip::routing_slip_sequence_= 0;
size_t Routing_Slip::count_enter_transient_ = 0;
size_t Routing_Slip::count_continue_transient_ = 0;
size_t Routing_Slip::count_enter_reloaded_ = 0;
size_t Routing_Slip::count_enter_new_ = 0;
size_t Routing_Slip::count_continue_new_ = 0;
size_t Routing_Slip::count_enter_complete_while_new_ = 0;
size_t Routing_Slip::count_enter_saving_ = 0;
size_t Routing_Slip::count_enter_saved_ = 0;
size_t Routing_Slip::count_enter_updating_ = 0;
size_t Routing_Slip::count_enter_changed_while_saving_ = 0;
size_t Routing_Slip::count_continue_changed_while_saving_ = 0;
size_t Routing_Slip::count_enter_changed_ = 0;
size_t Routing_Slip::count_continue_changed_ = 0;
size_t Routing_Slip::count_enter_complete_ = 0;
size_t Routing_Slip::count_enter_deleting_ = 0;
size_t Routing_Slip::count_enter_terminal_ = 0;

Routing_Slip_Ptr
Routing_Slip::create (const TAO_Notify_Event::Ptr& event)
{
  Routing_Slip * prs;
  ACE_NEW_THROW_EX (prs, Routing_Slip (event), CORBA::NO_MEMORY ());
  Routing_Slip_Ptr result(prs);
  result->this_ptr_ = result; // let the pointers touch so they use the same ref count

  // note we don't care about ultra-precise stats, so no guard for these
  if (DEBUG_LEVEL > 8 && ((result->sequence_ % 100) == 0))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Routing_Slip_Statistics\n")
      ACE_TEXT ("  enter_transient              \t%B\n")
      ACE_TEXT ("  continue_transient           \t%B\n")
      ACE_TEXT ("  enter_reloaded               \t%B\n")
      ACE_TEXT ("  enter_new                    \t%B\n")
      ACE_TEXT ("  continue_new                 \t%B\n")
      ACE_TEXT ("  enter_complete_while_new     \t%B\n")
      ACE_TEXT ("  enter_saving                 \t%B\n")
      ACE_TEXT ("  enter_saved                  \t%B\n")
      ACE_TEXT ("  enter_updating               \t%B\n")
      ACE_TEXT ("  enter_changed_while_saving   \t%B\n")
      ACE_TEXT ("  continue_changed_while_saving\t%B\n")
      ACE_TEXT ("  enter_changed                \t%B\n")
      ACE_TEXT ("  continue_changed             \t%B\n")
      ACE_TEXT ("  enter_complete               \t%B\n")
      ACE_TEXT ("  enter_deleting               \t%B\n")
      ACE_TEXT ("  enter_terminal               \t%B\n")
      , count_enter_transient_
      , count_continue_transient_
      , count_enter_reloaded_
      , count_enter_new_
      , count_continue_new_
      , count_enter_complete_while_new_
      , count_enter_saving_
      , count_enter_saved_
      , count_enter_updating_
      , count_enter_changed_while_saving_
      , count_continue_changed_while_saving_
      , count_enter_changed_
      , count_continue_changed_
      , count_enter_complete_
      , count_enter_deleting_
      , count_enter_terminal_
      ));
  }
  return result;
}

// static
Routing_Slip_Ptr
Routing_Slip::create (
  TAO_Notify_EventChannelFactory & ecf,
  Routing_Slip_Persistence_Manager * rspm)
{
  Routing_Slip_Ptr result;
  ACE_Message_Block * event_mb = 0;
  ACE_Message_Block * rs_mb = 0;
  try
    {
      if (rspm->reload (event_mb, rs_mb))
      {
        TAO_InputCDR cdr_event (event_mb);
        TAO_Notify_Event::Ptr event (TAO_Notify_Event::unmarshal (cdr_event));
        if (event.isSet())
        {
          result = create (event);
          TAO_InputCDR cdr_rs (rs_mb);
          if ( result->unmarshal (ecf, cdr_rs))
          {
            result->set_rspm (rspm);
          }
          else
          {
            ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("(%P|%t) Routing_Slip::create: Unmarshalling failed for routing slip.\n")
              ));
            result.reset ();
          }
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("(%P|%t) Routing_Slip::create: Unmarshalling failed for event.\n")
            ));
        }
      }
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Routing_Slip::create: Exception reloading event.\n")
        ));
    }
  delete event_mb;
  delete rs_mb;

  return result;
}

void
Routing_Slip::set_rspm (Routing_Slip_Persistence_Manager * rspm)
{
  this->rspm_ = rspm;
  if (rspm_ != 0)
  {
    rspm->set_callback (this);
  }
}

Routing_Slip::Routing_Slip(
      const TAO_Notify_Event::Ptr& event)
  : is_safe_ (false)
  , until_safe_ (internals_)
  , this_ptr_ (0)
  , event_(event)
  , state_ (rssCREATING)
  , complete_requests_ (0)
  , rspm_ (0)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, sequence_lock_);
  this->sequence_ = ++routing_slip_sequence_;
  if (DEBUG_LEVEL > 1) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: constructor\n"),
      this->sequence_
      ));
}

Routing_Slip::~Routing_Slip ()
{
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: destructor\n"),
      this->sequence_
      ));
}

bool
Routing_Slip::create_persistence_manager()
{
  if (this->rspm_ == 0)
  {
    Event_Persistence_Strategy * strategy =
      ACE_Dynamic_Service <TAO_Notify::Event_Persistence_Strategy>::instance ("Event_Persistence");
    if (strategy != 0)
    {
      Event_Persistence_Factory * factory = strategy->get_factory ();
      if (factory != 0)
      {
        set_rspm (factory->create_routing_slip_persistence_manager(this));
      }
    }
  }
  return this->rspm_ != 0;
}

const TAO_Notify_Event::Ptr &
Routing_Slip::event () const
{
  return this->event_;
}

void
Routing_Slip::wait_persist ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
  while (!this->is_safe_)
  {
    this->until_safe_.wait ();
  }
}

void
Routing_Slip::route (TAO_Notify_ProxyConsumer* pc, bool reliable_channel)
{
  ACE_ASSERT(pc != 0);

  TAO_Notify_ProxyConsumer::Ptr pcgrd(pc);

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  size_t const request_id = delivery_requests_.size ();

  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: add Delivery_Request #%B: lookup, completed %B of %B\n"),
      this->sequence_,
      request_id,
      this->complete_requests_,
      this->delivery_requests_.size ()));

  Delivery_Request_Ptr request (new Delivery_Request (this->this_ptr_, request_id));
  this->delivery_requests_.push_back (request);
  TAO_Notify_Method_Request_Lookup_Queueable method (request, pc);

  if (this->state_ == rssCREATING)
  {
    if (! reliable_channel)
    {
      enter_state_transient (guard);
    }
    else if (ACE_Dynamic_Service <TAO_Notify::Event_Persistence_Strategy>::instance ("Event_Persistence") == 0)
    {
      enter_state_transient (guard);
    }
    else if (! this->event_->reliable().is_valid())
    {
      enter_state_new (guard);
    }
    else if (this->event_->reliable().value() == true)
    {
      enter_state_new (guard);
    }
    else
    {
      enter_state_transient (guard);
    }
  }
  else
  {
    // We only need to release the guard if the state is rssCREATING.
    // By calling enter_state_*, we are guaranteed that the guard has
    // been released.
    guard.release ();
  }
  pc->execute_task (method);
}
#if 0 // forward
void
Routing_Slip::forward (TAO_Notify_ProxySupplier* ps, bool filter)
{
  // must be the first action
  ACE_ASSERT (this->state_ == rssCREATING);

  TAO_Notify_ProxySupplier::Ptr psgrd(ps);
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  enter_state_transient (guard);
  guard.acquire();
  size_t request_id = delivery_requests_.size ();

  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: add Delivery_Request #%d: Forward %s; completed %d of %d\n"),
      this->sequence_,
      static_cast<int> (request_id),
      filter ? ACE_TEXT ("Filter") : ACE_TEXT ("No Filter"),
      static_cast<int> (this->complete_requests_),
      static_cast<int> (this->delivery_requests_.size ())
      ));

  Delivery_Request_Ptr request (new Delivery_Request (this->this_ptr_, request_id));
  if (! ps->has_shutdown() )
    {
      this->delivery_requests_.push_back (request);
//      Delivery_Method_Dispatch method (request, ps, filter);
      TAO_Notify_Method_Request_Dispatch_No_Copy method (request, ps, filter);
      guard.release ();
      if (DEBUG_LEVEL > 8)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Routing Slip #%d: dispatching Delivery_Request %d to "
                    "proxy supplier %d\n",
                    this->sequence_,
                    static_cast<int> (request_id),
                    ps->id()));
      ps->worker_task()->execute (method);
    }
  else
    {
      if (DEBUG_LEVEL > 5)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Routing Slip #%d: not dispatching Delivery_Request %d to "
                    "proxy supplier %d; already shut down\n",
                    this->sequence_,
                    static_cast<int> (request_id),
                    ps->id()));
    }
}
#endif // forward

void
Routing_Slip::dispatch (
  TAO_Notify_ProxySupplier* ps,
  bool filter)
{
  // cannot be the first action
  ACE_ASSERT (this->state_ != rssCREATING);

  TAO_Notify_ProxySupplier::Ptr psgrd(ps);
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  size_t request_id = delivery_requests_.size ();

  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: add Delivery_Request #%B: Dispatch %s; completed %B of %B\n"),
      this->sequence_,
      request_id,
      filter ? ACE_TEXT ("Filter") : ACE_TEXT ("No Filter"),
      this->complete_requests_,
      this->delivery_requests_.size ()));

  Delivery_Request_Ptr request (new Delivery_Request (this->this_ptr_, request_id));
  if (! ps->has_shutdown() )
    {
      this->delivery_requests_.push_back (request);
      TAO_Notify_Method_Request_Dispatch_No_Copy method (request, ps, filter);
      guard.release ();
      if (DEBUG_LEVEL > 8)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Routing Slip #%d: dispatching Delivery_Request %B to "
                    "proxy supplier %d\n",
                    this->sequence_,
                    request_id,
                    ps->id()));
      ps->execute_task (method);
    }
  else
    {
      if (DEBUG_LEVEL > 5)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Routing Slip #%d: not dispatching Delivery_Request %B to "
                    "proxy supplier %d; already shut down\n",
                    this->sequence_,
                    request_id,
                    ps->id()));
    }
}

//////////
// signals

void
Routing_Slip::delivery_request_complete (size_t request_id)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
  ACE_ASSERT (request_id < this->delivery_requests_.size ());
  // reset the pointer to allow the delivery_request to be deleted.
  this->delivery_requests_[request_id].reset ();
  this->complete_requests_ += 1;

  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: delivery_request_complete #%B: completed %B of %B\n"),
      this->sequence_,
      request_id,
      this->complete_requests_,
      this->delivery_requests_.size ()));
  State state = this->state_;
  switch (state)
  {
    case rssTRANSIENT:
    {
      continue_state_transient (guard);
      break;
    }
    case rssNEW:
    {
      continue_state_new (guard);
      break;
    }
    case rssSAVING:
    {
      enter_state_changed_while_saving (guard);
      break;
    }
    case rssUPDATING:
    {
      enter_state_changed_while_saving (guard);
      break;
    }
    case rssSAVED:
    {
      enter_state_changed (guard);
      break;
    }
    case rssCHANGED_WHILE_SAVING:
    {
      continue_state_changed_while_saving (guard);
      break;
    }
    case rssCHANGED:
    {
      continue_state_changed (guard);
      break;
    }
    default:
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Notification Service Routing Slip: Unexpected delivery_request_complete in state %d\n"),
        static_cast<int> (this->state_)
        ));
      break;
    }
  }
}

void
Routing_Slip::at_front_of_persist_queue ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
  State state = this->state_;
  switch (state)
  {
    case rssNEW:
    {
      if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: NEW Reached front of queue\n"),
        this->sequence_
        ));
      enter_state_saving (guard);
      break;
    }
    case rssCOMPLETE_WHILE_NEW:
    {
      if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: COMPLETE_WHILE_NEW Reached front of queue\n"),
        this->sequence_
        ));
      guard.release ();
      this->persistent_queue_.complete ();
      enter_state_terminal (guard);
      break;
    }
    case rssCHANGED:
    {
      if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: CHANGED Reached front of queue\n"),
        this->sequence_
        ));
      enter_state_updating (guard);
      break;
    }
    case rssCOMPLETE:
    {
      if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: COMPLETE Reached front of queue\n"),
        this->sequence_
        ));
      enter_state_deleting (guard);
      break;
    }
    default:
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Routing Slip %d: Unexpected at_front_of_persist_queue in state %d\n"),
        this->sequence_,
        static_cast<int> (this->state_)
        ));
      break;
    }
  }
}

void
Routing_Slip::persist_complete ()
{
  // keep this object around til this method returns.
  Routing_Slip_Ptr me(this->this_ptr_);
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  // allow the ConsumerProxy to return from the CORBA push call.
  if (! is_safe_)
  {
    is_safe_ = true;
    this->until_safe_.signal ();
  }

  State state = this->state_;
  switch (state)
  {
    case rssSAVING:
    {
      if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: SAVING persist complete\n"),
        this->sequence_
        ));
      enter_state_saved(guard);
      break;
    }
    case rssCHANGED_WHILE_SAVING:
    {
      enter_state_changed (guard);
      break;
    }
    case rssUPDATING:
    {
      if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: UPDATING persist complete\n"),
        this->sequence_
        ));
      enter_state_saved (guard);
      break;
    }
    case rssDELETING:
    {
      if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: DELETING persist complete\n"),
        this->sequence_
        ));
      enter_state_terminal (guard);
      break;
    }
    default:
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Notification Service Routing Slip: Unexpected transition in state %d\n"),
        static_cast<int> (this->state_)
        ));
      guard.release ();
      break;
    }
  }
  this->persistent_queue_.complete ();
}

//////////////////
// support methods

bool
Routing_Slip::all_deliveries_complete () const
{
  return this->complete_requests_ == this->delivery_requests_.size ();
}

void
Routing_Slip::add_to_persist_queue(Routing_Slip_Guard & guard)
{
  guard.release ();
  this->persistent_queue_.add (this->this_ptr_);
}

////////////////////
// State transitions

void
Routing_Slip::enter_state_new (Routing_Slip_Guard & guard)
{
  ++count_enter_new_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state NEW\n"),
      this->sequence_
      ));
  this->state_ = rssNEW;
  add_to_persist_queue(guard);
}

void
Routing_Slip::continue_state_new (Routing_Slip_Guard & guard)
{
  ++count_continue_new_;
  if (all_deliveries_complete ())
  {
    this->enter_state_complete_while_new (guard);
  }
  guard.release ();
}
void
Routing_Slip::enter_state_complete_while_new (Routing_Slip_Guard & guard)
{
  ++count_enter_complete_while_new_;
  ACE_UNUSED_ARG (guard);
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state COMPLETE_WHILE_NEW\n"),
      this->sequence_
      ));
  // allow the ConsumerProxy to return from the CORBA push call.
  if (! is_safe_)
  {
    is_safe_ = true;
    this->until_safe_.signal ();
  }
  this->state_ = rssCOMPLETE_WHILE_NEW;
}

void
Routing_Slip::enter_state_reloaded (Routing_Slip_Guard & guard)
{
  ++count_enter_reloaded_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #&d: enter state RELOADED\n"),
      this->sequence_
      ));
  this->state_ = rssRELOADED;
  guard.release();
}

void
Routing_Slip::enter_state_transient (Routing_Slip_Guard & guard)
{
  ++count_enter_transient_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state TRANSIENT\n"),
      this->sequence_
      ));
  this->state_ = rssTRANSIENT;
  if (! is_safe_)
  {
    is_safe_ = true;
    this->until_safe_.signal ();
  }
  if (all_deliveries_complete ())
  {
    enter_state_terminal (guard);
  }
  else
  {
    guard.release ();
  }
}

void
Routing_Slip::continue_state_transient (Routing_Slip_Guard & guard)
{
  ++count_continue_transient_;
  if (all_deliveries_complete ())
  {
    enter_state_terminal (guard);
  }
  else
  {
    guard.release ();
  }
}
void
Routing_Slip::enter_state_saving (Routing_Slip_Guard & guard)
{
  ++count_enter_saving_;
  if (!create_persistence_manager ())
  {
    // Note This should actually be a throw (out of memory)
    // but we cheat and make this a transient event.
    guard.release ();
    this->persistent_queue_.complete ();
    enter_state_transient (guard);
  }
  else
  {
    if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state SAVING\n"),
        this->sequence_
        ));
    this->state_ = rssSAVING;

    TAO_OutputCDR event_cdr;
    this->event_->marshal (event_cdr);

    const ACE_Message_Block *event_mb = event_cdr.begin ();
    TAO_OutputCDR rs_cdr;
    marshal (rs_cdr);
    const ACE_Message_Block *rs_mb = rs_cdr.begin ();

    guard.release ();
    this->rspm_->store (*event_mb, *rs_mb);
  }
}

void
Routing_Slip::enter_state_saved (Routing_Slip_Guard & guard)
{
  ++count_enter_saved_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state SAVED\n"),
      this->sequence_
      ));
  this->state_ = rssSAVED;
  guard.release ();
}

void
Routing_Slip::enter_state_updating (Routing_Slip_Guard & guard)
{
  ++count_enter_updating_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state UPDATING\n"),
      this->sequence_
      ));
  this->state_ = rssUPDATING;

  TAO_OutputCDR rs_cdr;
  marshal (rs_cdr);
  const ACE_Message_Block *rs_mb = rs_cdr.begin ();
  guard.release ();

  ACE_ASSERT (this->rspm_ != 0);
  this->rspm_->update (*rs_mb);
}


void
Routing_Slip::enter_state_changed_while_saving (Routing_Slip_Guard & guard)
{
  ++count_enter_changed_while_saving_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state CHANGED_WHILE_SAVING\n"),
      this->sequence_
      ));
  this->state_ = rssCHANGED_WHILE_SAVING;
  guard.release ();
}

void
Routing_Slip::continue_state_changed_while_saving (Routing_Slip_Guard & guard)
{
  // no action necessary
  guard.release ();
}

void
Routing_Slip::enter_state_changed (Routing_Slip_Guard & guard)
{
  ++count_enter_changed_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state CHANGED\n"),
      this->sequence_
      ));
  // complete state change BEFORE initiating request to avoid
  // race condition if request finishes before state is stable.
  this->state_ = rssCHANGED;
  if (all_deliveries_complete ())
  {
    enter_state_complete (guard);
  }
  add_to_persist_queue (guard);
}

void
Routing_Slip::continue_state_changed (Routing_Slip_Guard & guard)
{
  ++count_continue_changed_;
  if (all_deliveries_complete ())
  {
    enter_state_complete (guard);
  }
  else
  {
    guard.release ();
  }
}

void
Routing_Slip::enter_state_complete (Routing_Slip_Guard & guard)
{
  ++count_enter_complete_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state COMPLETE\n"),
      this->sequence_
      ));
  this->state_ = rssCOMPLETE;
  guard.release ();
}

void
Routing_Slip::enter_state_deleting (Routing_Slip_Guard & guard)
{
  ++count_enter_deleting_;
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state DELETING\n"),
      this->sequence_
      ));
  this->state_ = rssDELETING;
  guard.release ();
  this->rspm_->remove ();
}

void
Routing_Slip::enter_state_terminal (Routing_Slip_Guard & guard)
{
  ++count_enter_terminal_;
  ACE_ASSERT( this->is_safe_);
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Routing Slip #%d: enter state TERMINAL\n"),
      this->sequence_
      ));
  this->state_ = rssTERMINAL;
  this->this_ptr_.reset ();
  guard.release ();
}

void
Routing_Slip::marshal (TAO_OutputCDR & cdr)
{
  size_t request_count = this->delivery_requests_.size();
  cdr.write_ulong (
    ACE_Utils::truncate_cast<CORBA::ULong> (request_count - this->complete_requests_));
  for (size_t nreq = 0; nreq < request_count; ++nreq)
  {
    Delivery_Request * request = this->delivery_requests_[nreq].get ();
    if (request != 0)
    {
      request->marshal (cdr);
    }
  }
}

bool
Routing_Slip::unmarshal (TAO_Notify_EventChannelFactory &ecf, TAO_InputCDR & cdr)
{
  CORBA::ULong count = 0;
  cdr.read_ulong (count);
  for (size_t nreq = 0; nreq < count; ++nreq)
  {
    ACE_CDR::Octet code = 0;
    while (cdr.read_octet(code))
    {
      try
      {
        if (code == TAO_Notify_Method_Request_Dispatch::persistence_code)
        {
          Delivery_Request * prequest;
          ACE_NEW_THROW_EX (
            prequest,
            Delivery_Request(this_ptr_, this->delivery_requests_.size ()),
            CORBA::NO_MEMORY ());
          Delivery_Request_Ptr request(prequest);
          TAO_Notify_Method_Request_Dispatch_Queueable * method =
            TAO_Notify_Method_Request_Dispatch::unmarshal (
              request,
              ecf,
              cdr);
          if (method != 0)
          {
            this->delivery_requests_.push_back (request);
            this->delivery_methods_.push_back (method);
          }
        }
        else if (code == TAO_Notify_Method_Request_Lookup::persistence_code)
        {
          Delivery_Request_Ptr request(new Delivery_Request(this_ptr_, this->delivery_requests_.size ()));
          TAO_Notify_Method_Request_Lookup_Queueable * method =
              TAO_Notify_Method_Request_Lookup::unmarshal (
                request,
                ecf,
                cdr);
          if (method != 0)
          {
            this->delivery_requests_.push_back (request);
            this->delivery_methods_.push_back (method);
          }
        }
      }
      catch (const CORBA::Exception&)
      {
        // @@todo should we log this?
        // just ignore failures
      }
    }
  }
  return this->delivery_requests_.size () > 0;
}

void
Routing_Slip::reconnect (void)
{
  Routing_Slip_Guard guard (this->internals_);
  enter_state_saved (guard);

  //@@todo is there a worker_task available to do this?
  size_t count = this->delivery_methods_.size ();
  for (size_t nmethod = 0; nmethod < count; ++nmethod)
  {
    this->delivery_methods_[nmethod]->execute ();
  }
  this->delivery_methods_.clear ();
}

int
Routing_Slip::sequence() const
{
  return this->sequence_;
}

bool
Routing_Slip::should_retry () const
{
  // simple minded test: if it's transient, don't retry it
  // @@todo Eventually this should check timeout, discard policy, etc.
  return this->state_ != rssTRANSIENT;
}

} // namespace

TAO_END_VERSIONED_NAMESPACE_DECL
