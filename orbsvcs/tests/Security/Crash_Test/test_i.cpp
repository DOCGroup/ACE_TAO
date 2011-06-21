// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

void
Simple_Server_i::send_line (const char *)
{

  /*
  static unsigned long count = 0;

  count++;

  if (count == 1)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Executed 128 requests. Now it's time to crash!\n"));
    ACE_OS::abort ();
  }
  */

  // ACE_DEBUG ((LM_DEBUG, "%s\n", line));
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
