/* -*- C++ -*- $Id$ */

#include "Notify_Dispatcher.h"
#include "ace/CORBA_macros.h"
#include "orbsvcs/orbsvcs/CosNotifyCommC.h" // i get some errors with env otherwise
#include "Notify_FilterAdmin_i.h"

TAO_Notify_Dispatcher::TAO_Notify_Dispatcher (void)
  :filter_admin_ (0)
{
}

TAO_Notify_Dispatcher::~TAO_Notify_Dispatcher ()
{
}

TAO_Notify_Dispatcher*
TAO_Notify_Dispatcher::create (CORBA::Environment &ACE_TRY_ENV)
{
  // just return the reactive dispatcher for now.
  // TODO: use the service configurator to allow the user to specify
  // which dispatcher to use and where.
  // @@ Should we have a factory instead?
  //
  TAO_Notify_Dispatcher* dispatcher;
  ACE_NEW_THROW_EX (dispatcher,
                    Notify_Reactive_Dispatcher (),
                    CORBA::NO_MEMORY());
  return dispatcher;
}

void
TAO_Notify_Dispatcher::set_FilterAdmin (TAO_Notify_FilterAdmin_i* filter_admin)
{
  filter_admin_ = filter_admin;
}

int
TAO_Notify_Dispatcher::add_dispatcher (TAO_Notify_Dispatcher& dispatcher)
{
  return contained_.insert (&dispatcher);
}

int
TAO_Notify_Dispatcher::remove_dispatcher (TAO_Notify_Dispatcher& dispatcher)
{
  return contained_.remove (&dispatcher);
}

void
Notify_Reactive_Dispatcher::dispatch_event (const CORBA::Any & data,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  // TODO: Filtering comes here..

  DISPATCHER_SET_ITER iter (contained_);
  TAO_Notify_Dispatcher **p_dispatcher;

  for (iter.first ();iter.done () != 1;iter.advance ())
    {
      if (iter.next (p_dispatcher) == 0)
        break;
      (*p_dispatcher)->dispatch_event (data, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
Notify_Reactive_Dispatcher::dispatch_event (
                                            const CosNotification::StructuredEvent& event,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  if (filter_admin_ != 0)
    {
      CORBA::Boolean truth =
        filter_admin_->match_structured (event, ACE_TRY_ENV);
      ACE_CHECK;

      if (truth == 0)
        return; // don't propogate the event further out.
    }

  DISPATCHER_SET_ITER iter (contained_);
  TAO_Notify_Dispatcher **p_dispatcher;

  for (iter.first ();iter.done () != 1;iter.advance ())
    {
      if (iter.next (p_dispatcher) == 0)
        break;
      (*p_dispatcher)->dispatch_event (event, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Set<TAO_Notify_Dispatcher *>;
template class ACE_Unbounded_Set_Iterator<TAO_Notify_Dispatcher *>;
template class ACE_Node <TAO_Notify_Dispatcher *>; 

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Set<TAO_Notify_Dispatcher *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Notify_Dispatcher *>;
#pragma instantiate ACE_Node <TAO_Notify_Dispatcher *>
#endif
