//
//$Id$
//

#include "RoundTrip_impl.h"

CORBA::ULongLong RoundTrip_impl::test_method (CORBA::ULongLong send_time)
{
  return send_time;
}
