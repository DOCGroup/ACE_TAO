// $Id$

#include "sum_server_i.h"

sum_server_i::sum_server_i ()
{
}

CORBA::Long
sum_server_i::add_variables (CORBA::Long a,
                             CORBA::Long b)
{
  return a+b;
}
