#include "Resume_Handle.h"
#include "ORB_Core.h"


#if !defined (__ACE_INLINE__)
# include "Resume_Handle.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Resume_Handle, "$Id$")

void
TAO_Resume_Handle::resume_handle (void)
{
  // If we have a complete message, just resume the handler
  // Resume the handler.
  // @@Bala: Try to solve this issue of reactor resumptions..
  if (this->flag_ == 0 &&
      this->handle_ != ACE_INVALID_HANDLE)
    this->orb_core_->reactor ()->resume_handler (this->handle_);

  // Set the flag to 1, so that we dont resume again..
  this->flag_ = 1;
}
