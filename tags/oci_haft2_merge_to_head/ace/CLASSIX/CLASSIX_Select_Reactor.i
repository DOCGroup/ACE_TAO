/* -*- C++ -*- */
// $Id$
//
// CLASSIX/Reactor.i
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
ACE_INLINE
ACE_CLASSIX_Select_Reactor_Notify::ACE_CLASSIX_Select_Reactor_Notify(void)
    : ACE_Reactor_Notify()
{
}

ACE_INLINE
ACE_CLASSIX_Select_Reactor_Notify::~ACE_CLASSIX_Select_Reactor_Notify(void)
{
}

ACE_INLINE
ACE_CLASSIX_Select_Reactor::ACE_CLASSIX_Select_Reactor(
    ACE_Timer_Queue *theTimeQ)
  : ACE_Select_Reactor(0, theTimeQ, 0,
		       new ACE_CLASSIX_Select_Reactor_Notify()),
    current_msg_size_ (0),
    current_handle_   (ACE_INVALID_HANDLE)
{
}

ACE_INLINE
ACE_CLASSIX_Select_Reactor::ACE_CLASSIX_Select_Reactor (
    size_t theSize,
    int theRs,
    ACE_Timer_Queue *theTimeQ)
  : ACE_Select_Reactor (theSize, theRs, 0, theTimeQ, 0,
			new ACE_CLASSIX_Select_Reactor_Notify() ),
    current_msg_size_ (0),
    current_handle_   (ACE_INVALID_HANDLE)
{
}

ACE_INLINE
int
ACE_CLASSIX_Select_Reactor_Notify::close (void)
{
  // deregister handle ???
  return this->notification_sap_.close ();
}

ACE_INLINE
void
ACE_CLASSIX_Select_Reactor_Notify::max_notify_iterations (int iterations)
{
}

ACE_INLINE
int
ACE_CLASSIX_Select_Reactor_Notify::max_notify_iterations (void)
{
    return 1;
}


/* ------------------------------------------------------------------------- */
