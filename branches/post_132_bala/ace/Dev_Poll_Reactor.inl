// -*- C++ -*-
//
// $Id$

ACE_INLINE
ACE_Dev_Poll_Event_Tuple::ACE_Dev_Poll_Event_Tuple (void)
  : event_handler (0),
    mask (ACE_Event_Handler::NULL_MASK),
    suspended (0),
    refcount (1)
{
}

// ---------------------------------------------------------------------

#if 0
ACE_INLINE
ACE_Dev_Poll_Ready_Set::ACE_Dev_Poll_Ready_Set (void)
  : pfds (0),
    nfds (0)
{
}
#endif  /* 0 */

// ---------------------------------------------------------------------

ACE_INLINE void
ACE_Dev_Poll_Reactor_Handler_Repository::mask (ACE_HANDLE handle,
                                               ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::mask");

  // Only bother to search for the handle if it's in range.
  if (this->handle_in_range (handle))
    this->handlers_[handle].mask = mask;
}

ACE_INLINE ACE_Reactor_Mask
ACE_Dev_Poll_Reactor_Handler_Repository::mask (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::mask");

  ACE_Reactor_Mask mask = ACE_Event_Handler::NULL_MASK;

  // Only bother to search for the handle if it's in range.
  if (this->handle_in_range (handle))
    mask = this->handlers_[handle].mask;

  if (mask == ACE_Event_Handler::NULL_MASK)
    errno = ENOENT;

  return mask;
}

ACE_INLINE void
ACE_Dev_Poll_Reactor_Handler_Repository::suspend (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::suspend");

  // Only bother to search for the handle if it's in range.
  if (this->handle_in_range (handle))
    this->handlers_[handle].suspended = 1;
}

ACE_INLINE void
ACE_Dev_Poll_Reactor_Handler_Repository::resume (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::resume");

  // Only bother to search for the handle if it's in range.
  if (this->handle_in_range (handle))
    this->handlers_[handle].suspended = 0;
}

ACE_INLINE int
ACE_Dev_Poll_Reactor_Handler_Repository::suspended (ACE_HANDLE handle) const
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::suspended");

  if (this->handle_in_range (handle))
    return this->handlers_[handle].suspended;

  return -1;
}

ACE_INLINE size_t
ACE_Dev_Poll_Reactor_Handler_Repository::size (void) const
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::size");

  return this->max_size_;
}

ACE_INLINE unsigned long
ACE_Dev_Poll_Reactor_Handler_Repository::add_ref (ACE_HANDLE handle)
{
  // ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::add_ref");

  // Caller provides synchronization

  if (this->handle_in_range (handle))
    return this->handlers_[handle].refcount++;

  return 0;
}

ACE_INLINE unsigned long
ACE_Dev_Poll_Reactor_Handler_Repository::remove_ref (ACE_HANDLE handle)
{
  // ACE_TRACE ("ACE_Dev_Poll_Reactor_Handler_Repository::remove_ref");

  // Caller provides synchronization

  if (this->handle_in_range (handle))
    {
      unsigned long & refcount = this->handlers_[handle].refcount;

      ACE_ASSERT  (refcount > 0);

      refcount--;

      if (refcount != 0)
        return refcount;

      // Reference count dropped to zero.  Remove the event handler
      // from the repository.
      this->unbind (handle);
    }

  return 0;
}

// -----------------------------------------------------------------

ACE_INLINE
ACE_Dev_Poll_Handler_Guard::ACE_Dev_Poll_Handler_Guard (
  ACE_Dev_Poll_Reactor_Handler_Repository &repository,
  ACE_HANDLE handle)
  : repository_ (repository),
    handle_ (handle)
{
  // Caller must provide synchronization.

  (void) repository.add_ref (handle);

  /**
   * @todo Suspend the handler so that other threads will not cause
   *       an event that is already in an upcall from being dispatched
   *       again.
   *
   * @note The naive approach would be to simply call
   *       suspend_handler_i() on the reactor.  However, that would
   *       cause a system call (write()) to occur.  Obviously this
   *       can potentially have an adverse affect on performance.
   *       Ideally, the handler would only be marked as "suspended" in
   *       the handler repository.  If an event arrives for a
   *       suspended handler that event can be "queued" in a
   *       "handle readiness queue."  "Queued" is quoted since a real
   *       queue need not be used since duplicate events can be
   *       coalesced, thus avoiding unbounded queue growth.  Event
   *       coalescing is already done by Linux's event poll driver
   *       (/dev/epoll) so Solaris' poll driver (/dev/poll) is the
   *       main concern here.  The largest the queue can be is the
   *       same size as the number of handlers stored in the handler
   *       repository.
   */
}

ACE_INLINE
ACE_Dev_Poll_Handler_Guard::~ACE_Dev_Poll_Handler_Guard (void)
{
  // Caller must provide synchronization.

  (void) this->repository_.remove_ref (this->handle_);

  /**
   * @todo Resume the handler so that other threads will be allowed to
   *       dispatch the handler.
   */
}

// ---------------------------------------------------------------------

ACE_INLINE int
ACE_Dev_Poll_Reactor::upcall (ACE_Event_Handler *event_handler,
                              int (ACE_Event_Handler::*callback)(ACE_HANDLE),
                              ACE_HANDLE handle)
{
  // If the handler returns positive value (requesting a reactor
  // callback) just call back as many times as the handler requests
  // it.  Other threads are off handling other things.
  int status = 0;

  do
    {
      status = (event_handler->*callback) (handle);
    }
  while (status > 0);

  return status;
}
