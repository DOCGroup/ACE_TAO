// $Id$

#include "RTCORBA.h"
#include "RT_ORB_Loader.h"

ACE_RCSID (RTCORBA, RTCORBA, "$Id$")

TAO_RTCORBA_Initializer::TAO_RTCORBA_Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_ORB_Loader);
}
