// $Id$

#include "Test_i.h"

ACE_RCSID(CORBA, Test_i, "$Id$")

void
Test_i::method (long input,
		ACE_CORBA_1 (Environment) &)
{
  ACE_DEBUG ((LM_DEBUG, "received a number %d\n", input));
}
