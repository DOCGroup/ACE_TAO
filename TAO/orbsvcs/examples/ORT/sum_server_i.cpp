#include "sum_server_i.h"

CORBA::Long
sum_server_i::add_variables (CORBA::Long a,
                             CORBA::Long b)
{
  return a+b;
}
