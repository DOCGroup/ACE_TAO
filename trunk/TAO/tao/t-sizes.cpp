// $Id$

#include "tao/corba.h"

#define SHOWSIZE(t) ACE_OS::printf ("sizeof (%s) = %u\n", #t, sizeof(t));

int
main (int, char*[])
{
  SHOWSIZE (CORBA::Boolean);
  SHOWSIZE (CORBA::Short);
  SHOWSIZE (CORBA::Long);
  SHOWSIZE (CORBA::LongLong);
  SHOWSIZE (CORBA::Float);
  SHOWSIZE (CORBA::Double);
  SHOWSIZE (CORBA::LongDouble);
}
