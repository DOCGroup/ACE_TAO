/**
 * @file Shared_Interceptor.h
 */
#ifndef Shared_Interceptor__h_
#define Shared_Interceptor__h_

#include "tao/IOPC.h"

// Unreserved (and probably unwanted) ServiceId
constexpr IOP::ServiceId service_id { 0xdeadbeef };

constexpr char magic_cookie[] = "Magic Cookie";
constexpr CORBA::ULong magic_cookie_len { sizeof (magic_cookie) };

#endif /* Shared_Interceptor__h_ */
