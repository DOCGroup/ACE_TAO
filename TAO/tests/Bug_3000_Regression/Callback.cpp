#include "Callback.h"

Callback::Callback ()
  : received_callback_ (false)
{
}

void
Callback::test_oneway ()
{
  received_callback_ = true;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Callback - test_oneway!\n"));
}

bool
Callback::received_callback ()
{
  return received_callback_;
}
