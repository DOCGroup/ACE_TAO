#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

void
Simple_Server_i::send_line (char const *)
{
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
