#include "LB_LoadAlertInfo.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"

ACE_RCSID (LoadBalancing,
           LB_LoadAlertInfo,
           "$Id$")

TAO_LB_LoadAlertInfo::TAO_LB_LoadAlertInfo (void)
  : load_alert (),
    location (),
    alerted (0)
{
}

int
TAO_LB_LoadAlertInfo::operator== (const TAO_LB_LoadAlertInfo & rhs)
{
  // For the purposes of the LoadAlert info set, only the location is
  // important.
  return this->location == rhs.location;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<TAO_LB_LoadAlertInfo>;
template class ACE_Unbounded_Set<TAO_LB_LoadAlertInfo>;
template class ACE_Unbounded_Set_Iterator<TAO_LB_LoadAlertInfo>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<TAO_LB_LoadAlertInfo>
#pragma instantiate ACE_Unbounded_Set<TAO_LB_LoadAlertInfo>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_LB_LoadAlertInfo>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
