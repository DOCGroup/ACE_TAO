// $Id$

#include "Event_Manager.h"

ACE_INLINE TAO_NS_Consumer_Map*
TAO_NS_Event_Manager::consumer_map (void)
{
  return consumer_map_;
}

ACE_INLINE TAO_NS_Supplier_Map*
TAO_NS_Event_Manager::supplier_map (void)
{
  return supplier_map_;
}
