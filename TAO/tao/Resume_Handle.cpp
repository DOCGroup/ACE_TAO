// -*- C++ -*-
// $Id$

#include "tao/Resume_Handle.h"
#include "tao/ORB_Core.h"
#include "debug.h"

#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "tao/Resume_Handle.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Resume_Handle::resume_handle (void)
{
  // If we have a complete message, just resume the handler
  // Resume the handler.
  if (this->orb_core_ &&
      this->orb_core_->reactor ()->resumable_handler () &&
      this->flag_ == TAO_HANDLE_RESUMABLE &&
      this->handle_ != ACE_INVALID_HANDLE)
    {
      this->orb_core_->reactor ()->resume_handler (this->handle_);
    }

  // Set the flag, so that we dont resume again..
  this->flag_ = TAO_HANDLE_ALREADY_RESUMED;
}

void
TAO_Resume_Handle::handle_input_return_value_hook (int& return_value)
{
  // RT8248: The return value is only changed from 1 to 0 if:
  //   1) the handle_input return value wants an immediate callback
  //      on the handle (i.e. will return "1")
  //   2) this->resume_handle was already called
  //   3) reactor->resume_handler was called by this->resume_handle
  // The value is changed because you can't ask for an immediate callback
  // on a handle that you have already given up ownership of. (RT8248)
  if ( return_value == 1 &&
       this->flag_ == TAO_HANDLE_ALREADY_RESUMED &&
       this->orb_core_ &&
       this->orb_core_->reactor ()->resumable_handler () &&
       this->handle_ != ACE_INVALID_HANDLE)
  {
    // a return value of "1" means "call me back immediately;
    // but we can't "call me back immediately" on an
    // already-resumed handle
    return_value = 0;

    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Resume_Handle::handle_input_return_value_hook, "
                  "overriding return value of 1 with retval = %d\n",
                  return_value));
    }
  }
  else if ( return_value == -1 )
  {
    // this covers the "connection close" case, where you want
    // to leave the handle suspended if you're return -1 to
    // remove the handle from the Reactor. (See ChangeLog entry
    // Fri Dec 16 14:40:54 2005)
    this->flag_ = TAO_HANDLE_LEAVE_SUSPENDED;

    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Resume_Handle::handle_input_return_value_hook, "
                  "handle_input returning -1, so handle is not resumed.\n"));
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
