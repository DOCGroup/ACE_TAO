#include "test_i.h"

void
test_i::send_stuff (char const * string)
{
  ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - %C\n", string));
}

