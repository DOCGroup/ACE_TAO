#include "Event_Reactor_Impl.h"
#include "Event_Wait_Strategy.h"
#include "IO_Handler_Repository.h"
#include "IO_Event_Handler.h"
#include "Event_Dispatcher.h"

#if !defined (__ACE_INLINE__)
#include "Event_Reactor.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao_ace, Event_Reactor_Impl, "$Id$")

TAO_ACE_Event_Reactor_Impl::TAO_ACE_Event_Reactor_Impl (void)
  : event_wait_strategy_ (0)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor_Impl::TAO_ACE_Event_Reactor_Impl");
}

TAO_ACE_Event_Reactor_Impl::~TAO_ACE_Event_Reactor_Impl (void)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor_Impl::~TAO_ACE_Event_Reactor_Impl");
}

int
TAO_ACE_Event_Reactor_Impl::register_io_handler (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor_Impl::register_io_handler");

  // @todo: Lock??

  return this->event_wait_strategy_->handler_repository ()->bind (eh,
                                                                  mask);
}

int
TAO_ACE_Event_Reactor_Impl::remove_io_handler (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor_Impl::remove_io_handler");

  // @todo: Lock??
  return this->event_wait_strategy_->handler_repository ()->unbind (eh,
                                                                    mask);

  // We need to call handle_close () or some such thing here
}

int
TAO_ACE_Event_Reactor_Impl::add_event_masks (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor_Impl::add_event_masks");

  // @todo: Lock??
  return this->event_wait_strategy_->handler_repository ()->add_masks
    (eh->get_handle (),
     mask);
}

int
TAO_ACE_Event_Reactor_Impl::clr_event_masks (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor_Impl::clr_event_masks");

  // @todo: Lock??
  return this->event_wait_strategy_->handler_repository ()->clr_masks
    (eh->get_handle (),
     mask);
}


int
TAO_ACE_Event_Reactor_Impl::handle_events (ACE_Time_Value * max_wait_time)
{
  // @@todo: Lock

  // Wait for events to arrive..

  int retval =
    this->event_wait_strategy_->wait_for_multiple_events (max_wait_time);


  // Handle timer events here

  // Handle IO events
  retval = this->handle_io_events ();
  return 0;
}



int
TAO_ACE_Event_Reactor_Impl::handle_io_events (void)
{

  TAO_ACE_IO_Event_Masks::Event_Masks t =
    TAO_ACE_IO_Event_Masks::NULL_MASK;

  // Select an event to dispatch
  ACE_HANDLE h =
    this->event_wait_strategy_->dispatchable_io_event (t);

  // Get the handle state
  TAO_ACE_IO_Handler_Repository::IO_Handler_State state;

  int retval =
    this->event_wait_strategy_->handler_repository ()->find_state (h,
                                                                   state);

  // Error in finding state
  if (retval == -1)
    return retval;

  if (state.marked_for_deletion_ == 0)
    {
      // Hand the handler state off to the dispatcher
      retval =
        this->event_dispatcher_->dispatch_io_events (
            state,
            t);
    }
}
