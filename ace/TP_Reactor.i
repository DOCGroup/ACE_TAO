/* -*- C++ -*- */
// $Id$

/************************************************************************/
// Methods for ACE_EH_Dispatch_Info
/************************************************************************/

ACE_INLINE
ACE_EH_Dispatch_Info::ACE_EH_Dispatch_Info (void)
{
  this->reset ();
}

ACE_INLINE void
ACE_EH_Dispatch_Info::set (ACE_HANDLE handle,
                           ACE_Event_Handler *event_handler,
                           ACE_Reactor_Mask mask,
                           ACE_EH_PTMF callback)
{
  this->dispatch_ = 1;

  this->handle_ = handle;
  this->event_handler_ = event_handler;
  this->mask_ = mask;
  this->callback_ = callback;
}

ACE_INLINE void
ACE_EH_Dispatch_Info::reset (void)
{
  this->dispatch_ = 0;

  this->handle_ = ACE_INVALID_HANDLE;
  this->event_handler_ = 0;
  this->mask_ = ACE_Event_Handler::NULL_MASK;
  this->callback_ = 0;
}

ACE_INLINE int
ACE_EH_Dispatch_Info::dispatch (void) const
{
  return this->dispatch_;
}

/************************************************************************/
// Methods for ACE_TP_Token_Guard
/************************************************************************/

ACE_INLINE
ACE_TP_Token_Guard::ACE_TP_Token_Guard (ACE_SELECT_REACTOR_TOKEN &token,
                                        ACE_Time_Value *max_wait_time,
                                        int &result)
  :token_ (token),
   owner_ (0)
{
  result = this->grab_token (max_wait_time);
}

ACE_INLINE
ACE_TP_Token_Guard::~ACE_TP_Token_Guard (void)
{
  if (this->owner_)
    {
      ACE_MT (this->token_.release ());
      this->owner_ = 0;
    }
}

ACE_INLINE void
ACE_TP_Token_Guard::release_token (void)
{
  ACE_MT (this->token_.release ());

  // We are not the owner anymore..
  this->owner_ = 0;
}


/************************************************************************/
// Methods for ACE_TP_Reactor
/************************************************************************/

ACE_INLINE int
ACE_TP_Reactor::handle_events (ACE_Time_Value &max_wait_time)
{
  return this->handle_events (&max_wait_time);
}

ACE_INLINE int
ACE_TP_Reactor::mask_ops (ACE_Event_Handler *eh,
                          ACE_Reactor_Mask mask,
                          int ops)
{
  return this->mask_ops (eh->get_handle (), mask, ops);
}
