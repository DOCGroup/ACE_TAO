#include "Test_i.h"
// @(#)Test_i.cpp	1.1	10/18/96


void
Test_i::method (long input,
		ACE_CORBA_1 (Environment) &)
{
  ACE_DEBUG ((LM_DEBUG, "received a number %d\n", input));
}
