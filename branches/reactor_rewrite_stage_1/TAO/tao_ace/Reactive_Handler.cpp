#include "Reactive_Handler.h"


#if !defined (__ACE_INLINE__)
#include "Reactive_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao_ace, Reactive_Handler, "$Id$")

TAO_ACE_Reactive_Handler::TAO_ACE_Reactive_Handler (long priority)
  : ref_count_ (),
    priority_ (priority)
{
  ACE_TRACE ("TAO_ACE_Reactive_Handler::TAO_ACE_Reactive_Handler");
  // Set the reference count to be 1. Doing so we implicitly assume
  // that our creator has a refernce to us, whoever it may be.
  this->ref_count_ = 1;
}

TAO_ACE_Reactive_Handler::~TAO_ACE_Reactive_Handler (void)
{
  ACE_TRACE ("TAO_ACE_Reactive_Handler::~TAO_ACE_Reactive_Handler");
}
