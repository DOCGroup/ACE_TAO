#include "Event_Dispatcher.h"
#include "Event_Reactor_Impl.h"
#include "Event_Wait_Strategy.h"
#include "IO_Event_Handler.h"
#include "ace/Log_Msg.h"

ACE_RCSID(tao_ace, Event_Dispatcher, "$Id$")


TAO_ACE_Event_Dispatcher::TAO_ACE_Event_Dispatcher (
    TAO_ACE_Event_Reactor_Impl *reactor)
  : reactor_impl_ (reactor)
{

}

int
TAO_ACE_Event_Dispatcher::dispatch_io_events (
    TAO_ACE_IO_Handler_Repository::IO_Handler_State &state,
    TAO_ACE_IO_Event_Masks::Event_Masks &t)
{
  // If marked for deletion just return a -1.
  if (state.marked_for_deletion_)
    return -1;

  // Increase the thread count
  ++state.thread_count_;

  int retval =
    this->reactor_impl_->event_wait_strategy_->handler_repository ()->rebind_state (state);

  // Put the state object into the repository.
  if (retval < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Rebind after change of state failed \n"));

      return -1;
    }

  // @todo: Release the lock here..

  // Make the callback
  if (state.event_handler_->handle_event (t) < 0)
    {
      retval =
        this->reactor_impl_->remove_io_handler (state.event_handler_,
                                                t);

      return -1;
    }

  return 0;
}
