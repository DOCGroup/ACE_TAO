#include "Select_Wait_Strategy.h"
#include "Event_Selection_Strategy.h"
#include "IO_Event_Handler.h"


ACE_RCSID(tao_ace, Select_Wait_Strategy, "$Id$")

TAO_ACE_IO_Handler_Repository *
TAO_ACE_Select_Wait_Strategy::handler_repository (void)
{
  ACE_TRACE ("TAO_ACE_Select_Wait_Strategy::handle_repository");

  return &this->io_handler_repo_;
}

ACE_HANDLE
TAO_ACE_Select_Wait_Strategy::dispatchable_io_event (
    TAO_ACE_IO_Event_Masks::Event_Masks &t)
{
  ACE_TRACE ("TAO_ACE_Select_Wait_Strategy::dispatchable_io_event");

  return this->event_selection_strategy_->dispatchable_io_event (t);
}

int
TAO_ACE_Select_Wait_Strategy::wait_for_multiple_events (
    ACE_Time_Value *max_wait_time)
{
  ACE_TRACE ("TAO_ACE_Select_Wait_Strategy::wait_for_multiple_events");

  int no_active_handles = 0;

  // Check the strategy
  if (!this->event_selection_strategy_->event_reselect () &&
      ((no_active_handles = this->active_handles ()) > 0))
    {
      return no_active_handles;
    }

  // We need to handle events that have not been handled before...
  do
    {
      // @@<todo>: Do timeouts calculations here
      ACE_Time_Value *timeout = max_wait_time;

      // Get the wait set from the repo
      TAO_ACE_Select_Strategy_Handle_Set &wait_set =
        this->io_handler_repo_.wait_set ();

      // Make a copy of the latest set bits..
      this->ready_set_.rd_mask_ = wait_set.rd_mask_;
      this->ready_set_.wr_mask_ = wait_set.wr_mask_;
      this->ready_set_.ex_mask_ = wait_set.ex_mask_;

      // Do the select now
      no_active_handles = ACE_OS::select (this->io_handler_repo_.max_handlep1 (),
                                          this->ready_set_.rd_mask_,
                                          this->ready_set_.wr_mask_,
                                          this->ready_set_.ex_mask_,
                                          timeout);
    }
  while (no_active_handles == -1 && this->handle_error () > 0);

  if (no_active_handles > 0)
    {
#if !defined (ACE_WIN32)

      // Resynchronize the fd_sets so their "max" is set properly.
      this->ready_set_.rd_mask_.sync (this->io_handler_repo_.max_handlep1 ());
      this->ready_set_.wr_mask_.sync (this->io_handler_repo_.max_handlep1 ());
      this->ready_set_.ex_mask_.sync (this->io_handler_repo_.max_handlep1 ());
#endif /* ACE_WIN32 */
    }

  // Return the number of events to dispatch.
  return no_active_handles;
}


int
TAO_ACE_Select_Wait_Strategy::active_handles (void)
{
  ACE_TRACE ("TAO_ACE_Select_Wait_Strategy::active_handles");

# if 0
  if (this->mask_signals_)
    {
#if !defined (ACE_WIN32)*/
      // Make this call signal safe.
      ACE_Sig_Guard sb;
#endif /* ACE_WIN32 */
    }
#endif /*if 0*/

  return this->ready_set_.rd_mask_.num_set ()
    + this->ready_set_.wr_mask_.num_set ()
    + this->ready_set_.ex_mask_.num_set ();
}

int
TAO_ACE_Select_Wait_Strategy::handle_error (void)
{
  ACE_TRACE ("TAO_ACE_Select_Wait_Strategy::handle_error");

  if (errno == EINTR)
    return 1;
#if defined (__MVS__) || defined (ACE_WIN32) || defined (VXWORKS)
  // On MVS Open Edition and Win32, there can be a number of failure
  // codes on a bad socket, so check_handles on anything other than
  // EINTR.  VxWorks doesn't even bother to always set errno on error
  // in select (specifically, it doesn't return EBADF for bad FDs).
  else
    return this->check_handles ();
#else
#  if defined (ACE_PSOS)
  else if (errno == EBADS)
    return this->check_handles ();
#  else
  else if (errno == EBADF)
    return this->check_handles ();
#  endif /* ACE_PSOS */
  else
    return -1;
#endif  /* __MVS__ || ACE_WIN32 */

}

int
TAO_ACE_Select_Wait_Strategy::check_handles (void)
{
  ACE_TRACE ("TAO_ACE_Select_Wait_Strategy::check_handles");

#if defined (ACE_WIN32) || defined (__MVS__) || defined (ACE_PSOS) || defined (VXWORKS)
  ACE_Time_Value time_poll = ACE_Time_Value::zero;
  ACE_Handle_Set rd_mask;
#endif /* ACE_WIN32 || MVS || ACE_PSOS || VXWORKS */

  TAO_ACE_IO_Handler_Repository::IO_Handler_State *state = 0;
  int result = 0;

  for (TAO_ACE_Select_IO_Handler_Repository_Iterator iter (this->io_handler_repo_);
       iter.next (state) != 0;
       iter.advance ())
    {
      ACE_HANDLE handle =
        state->event_handler_->get_handle ();

      // Skip back to the beginning of the loop if the HANDLE is
      // invalid.
      if (handle == ACE_INVALID_HANDLE)
        continue;

#if defined (ACE_WIN32) || defined (__MVS__) || defined (ACE_PSOS) || defined (VXWORKS)
      // Win32 needs to do the check this way because fstat won't work on
      // a socket handle.  MVS Open Edition needs to do it this way because,
      // even though the docs say to check a handle with either select or
      // fstat, the fstat method always says the handle is ok.
      // pSOS needs to do it this way because file handles and socket handles
      // are maintained by separate pieces of the system.  VxWorks needs the select
      // variant since fstat always returns an error on socket FDs.
      rd_mask.set_bit (handle);

      if (ACE_OS::select (int (handle) + 1,
                          rd_mask, 0, 0,
                          &time_poll) < 0)
        {
          result = 1;
          this->io_handler_repo_.unbind (state->evenet_handler_,
                                         TAO_ACE_Event_Masks::ALL_EVENTS_MASK);
        }
      rd_mask.clr_bit (handle);
#else /* !ACE_WIN32 && !MVS && !ACE_PSOS */
      struct stat temp;

      if (ACE_OS::fstat (handle, &temp) == -1)
        {
          result = 1;
          this->io_handler_repo_.unbind (state->event_handler_,
                                         TAO_ACE_IO_Event_Masks::ALL_EVENTS_MASK);
        }
#endif /* ACE_WIN32 || MVS || ACE_PSOS */
    }

  return result;
}
