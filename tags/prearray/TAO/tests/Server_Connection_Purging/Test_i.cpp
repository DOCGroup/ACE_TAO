//
// $Id$
//
#include "Test_i.h"

void
test_i::send_stuff (const char* string
                    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - %s\n", string));
}
