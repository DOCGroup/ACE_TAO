/**
 * @file Shared_Interceptor.h
 *
 * $Id$
 *
 */
#ifndef Shared_Interceptor__h_
#define Shared_Interceptor__h_

#include "tao/IOPC.h"

// Unreserved (and probably unwanted) ServiceId
const IOP::ServiceId service_id = 0xdeadbeef;

const char magic_cookie[] = "Magic Cookie";
const CORBA::ULong magic_cookie_len = sizeof(magic_cookie);

#endif /* Shared_Interceptor__h_ */
