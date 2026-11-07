#include "Callback.h"

void
Callback::test_oneway ()
{
  received_callback_ = true;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Callback - test_oneway!\n"));
}

bool
Callback::received_callback () const
{
  return received_callback_;
}
