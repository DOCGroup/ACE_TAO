// $Id$

#include "RTCORBA.h"
#include "RT_ORB_Loader.h"
#include "RT_Protocols_Hooks.h"

ACE_RCSID (RTCORBA, RTCORBA, "$Id$")

static TAO_RTCORBA_Initializer TAO_RTCORBA_initializer;

TAO_RTCORBA_Initializer::TAO_RTCORBA_Initializer (void)
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_TAO_RT_ORB_Loader);
}
