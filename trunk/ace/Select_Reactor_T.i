/* -*- C++ -*- */
// $Id$

#include "ace/Reactor.h"

template <class ACE_SELECT_REACTOR_TOKEN> ACE_INLINE int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::resume_handler (const ACE_Handle_Set &handles)
{
  ACE_TRACE ("ACE_Select_Reactor_T::resume_handler");
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;

  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->resume_i (h) == -1)
      return -1;

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> ACE_INLINE int
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::suspend_handler (const ACE_Handle_Set &handles)
{
  ACE_TRACE ("ACE_Select_Reactor_T::suspend_handler");
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;

  ACE_MT (ACE_GUARD_RETURN (ACE_SELECT_REACTOR_TOKEN, ace_mon, this->token_, -1));

  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->suspend_i (h) == -1)
      return -1;

  return 0;
}

template <class ACE_SELECT_REACTOR_TOKEN> ACE_INLINE ACE_Lock &
ACE_Select_Reactor_T<ACE_SELECT_REACTOR_TOKEN>::lock (void)
{
  ACE_TRACE ("ACE_Select_Reactor_T::lock");
  return this->lock_adapter_;
}

