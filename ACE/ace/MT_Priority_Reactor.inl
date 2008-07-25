// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/************************************************************************/
// Methods for ACE_Handle_Dispatch_Info
/************************************************************************/

ACE_INLINE
ACE_Handle_Dispatch_Info::ACE_Handle_Dispatch_Info (void) :
  handle_ (ACE_INVALID_HANDLE),
  event_handler_ (0),
  mask_ (ACE_Event_Handler::NULL_MASK),
  callback_ (0),
  resume_flag_ (ACE_Event_Handler::ACE_REACTOR_RESUMES_HANDLER),
  reference_counting_required_ (false),
  dispatch_ (false)
{
}

ACE_INLINE void
ACE_Handle_Dispatch_Info::set (ACE_HANDLE handle,
                           ACE_Event_Handler *event_handler,
                           ACE_Reactor_Mask mask,
                           ACE_EH_PTMF callback)
{
  this->dispatch_ = true;

  this->handle_ = handle;
  this->event_handler_ = event_handler;
  this->mask_ = mask;
  this->callback_ = callback;
  if (event_handler_)
    {
      this->resume_flag_ = event_handler->resume_handler ();
      this->reference_counting_required_ =
        (event_handler_->reference_counting_policy ().value () ==
         ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
    }
  else
    this->dispatch_ = false;
}

ACE_INLINE bool
ACE_Handle_Dispatch_Info::dispatch (void) const
{
  return this->dispatch_;
}


/************************************************************************/
// Methods for ACE_MT_Priority_Reactor
/************************************************************************/

ACE_INLINE void
ACE_MT_Priority_Reactor::no_op_sleep_hook (void *)
{
}

ACE_INLINE void
ACE_MT_Priority_Reactor::clear_read_set (ACE_HANDLE handle)
{
  this->ready_set_.wr_mask_.clr_bit (handle);
  this->ready_set_.ex_mask_.clr_bit (handle);
  this->ready_set_.rd_mask_.clr_bit (handle);
}

ACE_INLINE void
ACE_MT_Priority_Reactor::clear_dispatch_mask (ACE_HANDLE ,
                                     ACE_Reactor_Mask )
{
  this->ready_set_.rd_mask_.reset ();
  this->ready_set_.wr_mask_.reset ();
  this->ready_set_.ex_mask_.reset ();
}

ACE_END_VERSIONED_NAMESPACE_DECL
