// $Id$

#include "RTScheduler.h"
#include "RTScheduler_Loader.h"

ACE_RCSID (RTScheduler, RTScheduler, "$Id$")

TAO_RTScheduler_Initializer::TAO_RTScheduler_Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RTScheduler_Loader);
}
