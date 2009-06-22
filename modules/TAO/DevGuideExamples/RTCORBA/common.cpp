// $Id$

#include "common.h"

CORBA::ULong
get_total_lanes()
{
#if defined (linux)
  return 1;
#elif defined (ACE_CONFIG_WIN32_H)
  return 3;
#else
  return 5;
#endif
}

CORBA::Short
get_increment()
{
  return 1;
}

