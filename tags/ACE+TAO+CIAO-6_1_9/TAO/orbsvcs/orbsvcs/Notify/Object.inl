// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Long
TAO_Notify_Object::id (void) const
{
  return id_;
}

ACE_INLINE void
TAO_Notify_Object::execute_task (TAO_Notify_Method_Request& method_request)
{
  TAO_Notify_Worker_Task::Ptr task( this->worker_task_ );
  if ( task.isSet() )
  {
    task->execute( method_request );
  }
}

ACE_INLINE void
TAO_Notify_Object::inherit_poas (TAO_Notify_Object& parent)
{
  this->set_proxy_poa( parent.proxy_poa() );
  this->set_object_poa( parent.object_poa() );
  this->set_poa( parent.poa() );

  // Do not take ownership of parent's poas
  own_proxy_poa_ = false;
  own_object_poa_ = false;
}

ACE_INLINE void
TAO_Notify_Object::adopt_poa (TAO_Notify_POA_Helper* single)
{
  ACE_ASSERT( single != 0 );
  this->set_proxy_poa( single );
  this->set_object_poa( single );
  this->set_poa( single );

  // Maintain ownership of the poa
}

ACE_INLINE void
TAO_Notify_Object::set_primary_as_proxy_poa()
{
  this->set_poa( this->proxy_poa() );
}

ACE_INLINE TAO_Notify_POA_Helper*
TAO_Notify_Object::poa (void)
{
  return this->poa_;
}

ACE_INLINE TAO_Notify_POA_Helper*
TAO_Notify_Object::object_poa (void)
{
  return this->object_poa_;
}

ACE_INLINE TAO_Notify_POA_Helper*
TAO_Notify_Object::proxy_poa (void)
{
  return this->proxy_poa_;
}

ACE_INLINE bool
TAO_Notify_Object::has_shutdown (void)
{
  return this->shutdown_;
}

ACE_INLINE void
TAO_Notify_Object::set_admin_properties( TAO_Notify_AdminProperties* admin_properties )
{
  ACE_ASSERT( admin_properties != 0 );
  this->admin_properties_.reset( admin_properties );
}

ACE_INLINE TAO_Notify_Event_Manager&
TAO_Notify_Object::event_manager (void)
{
  ACE_ASSERT( this->event_manager_.get() != 0 );
  return *this->event_manager_;
}

ACE_INLINE TAO_Notify_AdminProperties&
TAO_Notify_Object::admin_properties (void)
{
  ACE_ASSERT( this->admin_properties_.get() != 0 );
  return *this->admin_properties_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
