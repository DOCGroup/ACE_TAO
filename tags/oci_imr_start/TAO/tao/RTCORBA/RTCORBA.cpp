// $Id$

#include "RTCORBA.h"

#if TAO_HAS_CORBA_MESSAGING == 1

#include "RT_ORB_Loader.h"

ACE_RCSID (RTCORBA, RTCORBA, "$Id$")

TAO_RTCORBA_Initializer::TAO_RTCORBA_Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_ORB_Loader);
}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
