/* -*- C++ -*- */
// $Id$

/************************************************************************/
// Methods for ACE_EH_Dispatch_Info
/************************************************************************/

ACE_INLINE void
ACE_EH_Dispatch_Info::reset (void)
{
  this->dispatch_ = 0;

  this->handle_ = ACE_INVALID_HANDLE;
  this->event_handler_ = 0;
  this->mask_ = ACE_Event_Handler::NULL_MASK;
  this->callback_ = 0;
}


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

ACE_INLINE int
ACE_EH_Dispatch_Info::dispatch (void) const
{
  return this->dispatch_;
}

/************************************************************************/
// Methods for ACE_TP_Token_Guard
/************************************************************************/

ACE_INLINE
ACE_TP_Token_Guard::ACE_TP_Token_Guard (ACE_Select_Reactor_Token &token)

  : token_ (token),
    owner_ (0)
{
}

ACE_INLINE
ACE_TP_Token_Guard::~ACE_TP_Token_Guard (void)
{
  if (this->owner_ == 1)
    {
      ACE_MT (this->token_.release ());
      this->owner_ = 0;
    }
}

ACE_INLINE void
ACE_TP_Token_Guard::release_token (void)
{
  if (this->owner_)
    {
      ACE_MT (this->token_.release ());

      // We are not the owner anymore..
      this->owner_ = 0;
    }
}

ACE_INLINE int
ACE_TP_Token_Guard::is_owner (void)
{
  return this->owner_;
}


/************************************************************************/
// Methods for ACE_TP_Reactor
/************************************************************************/

ACE_INLINE void
ACE_TP_Reactor::no_op_sleep_hook (void *)
{
}

ACE_INLINE void
ACE_TP_Reactor::clear_handle_read_set (ACE_HANDLE handle)
{
  this->ready_set_.wr_mask_.clr_bit (handle);
  this->ready_set_.ex_mask_.clr_bit (handle);
  this->ready_set_.rd_mask_.clr_bit (handle);
}
