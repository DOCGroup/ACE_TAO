// $Id$

ACE_INLINE CORBA::Long
TAO_NS_Object::id (void)
{
  return id_;
}

ACE_INLINE TAO_NS_Worker_Task*
TAO_NS_Object::worker_task (void)
{
  return this->worker_task_;
}

ACE_INLINE TAO_NS_POA_Helper*
TAO_NS_Object::poa (void)
{
  return this->poa_;
}

ACE_INLINE TAO_NS_POA_Helper*
TAO_NS_Object::object_poa (void)
{
  return this->object_poa_;
}

ACE_INLINE TAO_NS_POA_Helper*
TAO_NS_Object::proxy_poa (void)
{
  return this->proxy_poa_;
}

ACE_INLINE int
TAO_NS_Object::has_shutdown (void)
{
  return this->shutdown_;
}

ACE_INLINE TAO_NS_Event_Manager*
TAO_NS_Object::event_manager (void)
{
  return this->event_manager_;
}
