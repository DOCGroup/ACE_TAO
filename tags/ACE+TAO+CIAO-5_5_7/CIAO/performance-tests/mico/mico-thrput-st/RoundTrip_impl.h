//
//$Id$
//
#include "RoundTrip.h"

class RoundTrip_impl : virtual public POA_Roundtrip {
 public:
  CORBA::ULongLong test_method (CORBA::ULongLong send_time);
};
