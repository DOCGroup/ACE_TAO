// $Id$

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

ACE_INLINE TAO_NS_Event_Dispatch_Observer*
TAO_NS_Event_Manager::event_dispatch_observer (void)
{
  return this->event_dispatch_observer_;
}

ACE_INLINE TAO_NS_Updates_Dispatch_Observer*
TAO_NS_Event_Manager::updates_dispatch_observer (void)
{
  return this->updates_dispatch_observer_;
}
