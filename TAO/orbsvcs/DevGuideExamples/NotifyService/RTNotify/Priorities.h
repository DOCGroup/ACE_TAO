#ifndef _PRIORITIES_H_
#define _PRIORITIES_H_

#include "tao/RTCORBA/RTCORBA.h"

inline constexpr RTCORBA::Priority LOW_PRIORITY {};
inline constexpr RTCORBA::Priority HIGH_PRIORITY {};
inline constexpr RTCORBA::Priority DEFAULT_PRIORITY = LOW_PRIORITY;

#endif
