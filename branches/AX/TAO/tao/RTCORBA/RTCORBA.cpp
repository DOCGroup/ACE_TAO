// $Id$

#include "RTCORBA.h"
#include "RT_ORB_Loader.h"
#include "RT_Protocols_Hooks.h"

ACE_RCSID (RTCORBA, RTCORBA, "$Id$")

TAO_RTCORBA_Initializer::TAO_RTCORBA_Initializer (void)
{
  TAO_RTCORBA_Initializer::init ();
}

int
TAO_RTCORBA_Initializer::init (void)
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_RT_ORB_Loader);
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_RT_Protocols_Hooks);
  return 0;
}
