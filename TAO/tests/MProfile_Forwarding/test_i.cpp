#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

Simple_Server_i::Simple_Server_i ()
  :orb_ (0)
{
}

void
Simple_Server_i::remote_call ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Print out from process id (%P) hosting the servant\n"));
}


void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
