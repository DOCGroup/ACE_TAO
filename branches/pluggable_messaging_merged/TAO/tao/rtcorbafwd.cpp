// $Id$

#include "tao/rtcorbafwd.h"

#if defined (TAO_HAS_RT_CORBA)

#if !defined (__ACE_INLINE__)
# include "tao/rtcorbafwd.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, rtcorbafwd, "$Id$")

TAO_NAMESPACE_TYPE (const RTCORBA::Priority)
TAO_NAMESPACE_BEGIN (RTCORBA)
TAO_NAMESPACE_DEFINE (const RTCORBA::Priority, minPriority, 0)
TAO_NAMESPACE_END

TAO_NAMESPACE_TYPE (const RTCORBA::Priority)
TAO_NAMESPACE_BEGIN (RTCORBA)
TAO_NAMESPACE_DEFINE (const RTCORBA::Priority, maxPriority, 32767)
TAO_NAMESPACE_END

#endif /* TAO_HAS_RT_CORBA */
