#include "LB_LoadAlertInfo.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"

ACE_RCSID (LoadBalancing,
           LB_LoadAlertInfo,
           "$Id$")

TAO_LB_LoadAlertInfo::TAO_LB_LoadAlertInfo (void)
  : load_alert (),
    alerted (0)
{
}
