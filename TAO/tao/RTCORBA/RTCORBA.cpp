// $Id$

#include "tao/RTCORBA/RTCORBA.h"

#if TAO_HAS_CORBA_MESSAGING == 1

#include "tao/RTCORBA/RT_ORB_Loader.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RTCORBA_Initializer::TAO_RTCORBA_Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_ORB_Loader);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
