#include "Resume_Handle.h"
#include "ORB_Core.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "Resume_Handle.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Resume_Handle, "$Id$")

void
TAO_Resume_Handle::resume_handle (void)
{
  // If we have a complete message, just resume the handler
  // Resume the handler.
  if (this->orb_core_ &&
      this->orb_core_->reactor ()->resumable_handler () &&
      this->flag_ == TAO_HANDLE_RESUMABLE &&
      this->handle_ != ACE_INVALID_HANDLE)
    this->orb_core_->reactor ()->resume_handler (this->handle_);

  // Set the flag, so that we dont resume again..
  this->flag_ = TAO_HANDLE_ALREADY_RESUMED;
}
