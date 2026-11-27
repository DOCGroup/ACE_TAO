#include "INS_i.h"

char *
INS_i::test_ins ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Inside Operation\n")));
  return CORBA::string_dup ("Success");
}
