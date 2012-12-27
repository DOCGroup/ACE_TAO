// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Notify_Factory*
TAO_Notify_Properties::factory (void)
{
  return this->factory_;
}

ACE_INLINE void
TAO_Notify_Properties::factory (TAO_Notify_Factory* factory)
{
  this->factory_ = factory;
}

ACE_INLINE TAO_Notify_Builder*
TAO_Notify_Properties::builder (void)
{
  return this->builder_;
}

ACE_INLINE void
TAO_Notify_Properties::builder (TAO_Notify_Builder* builder)
{
  this->builder_ = builder;
}

ACE_INLINE CORBA::ORB_ptr
TAO_Notify_Properties::orb (void)
{
  return CORBA::ORB::_duplicate (orb_.in ());
}

ACE_INLINE CORBA::ORB_ptr
TAO_Notify_Properties::dispatching_orb (void)
{
  return CORBA::ORB::_duplicate (dispatching_orb_.in ());
}

ACE_INLINE void
TAO_Notify_Properties::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

ACE_INLINE void
TAO_Notify_Properties::dispatching_orb (CORBA::ORB_ptr dispatching_orb)
{
  dispatching_orb_ = CORBA::ORB::_duplicate (dispatching_orb);
}

ACE_INLINE PortableServer::POA_ptr
TAO_Notify_Properties::default_poa (void)
{
  return PortableServer::POA::_duplicate (this->default_poa_.in ());
}

ACE_INLINE void
TAO_Notify_Properties::default_poa (PortableServer::POA_ptr default_poa)
{
  this->default_poa_ = PortableServer::POA::_duplicate (default_poa);
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Properties::asynch_updates (void)
{
  return this->asynch_updates_;
}

ACE_INLINE void
TAO_Notify_Properties::asynch_updates (CORBA::Boolean asynch_updates)
{
  this->asynch_updates_ = asynch_updates;
}

ACE_INLINE bool
TAO_Notify_Properties::allow_reconnect (void)
{
  return this->allow_reconnect_;
}

ACE_INLINE void
TAO_Notify_Properties::allow_reconnect (bool b)
{
  this->allow_reconnect_ = b;
}

ACE_INLINE bool
TAO_Notify_Properties::validate_client (void)
{
  return this->validate_client_;
}

ACE_INLINE void
TAO_Notify_Properties::validate_client (bool b)
{
  this->validate_client_ = b;
}

ACE_INLINE ACE_Time_Value
TAO_Notify_Properties::validate_client_delay (void)
{
  return this->validate_client_delay_;
}

ACE_INLINE void
TAO_Notify_Properties::validate_client_delay (ACE_Time_Value b)
{
  this->validate_client_delay_ = b;
}

ACE_INLINE ACE_Time_Value
TAO_Notify_Properties::validate_client_interval (void)
{
  return this->validate_client_interval_;
}

ACE_INLINE void
TAO_Notify_Properties::validate_client_interval (ACE_Time_Value b)
{
  this->validate_client_interval_ = b;
}


ACE_INLINE bool
TAO_Notify_Properties::separate_dispatching_orb (void)
{
  return this->separate_dispatching_orb_;
}

ACE_INLINE void
TAO_Notify_Properties::separate_dispatching_orb (bool b)
{
  this->separate_dispatching_orb_ = b;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Properties::updates (void)
{
  return this->updates_;
}

ACE_INLINE void
TAO_Notify_Properties::updates (CORBA::Boolean updates)
{
  this->updates_ = updates;
}

ACE_INLINE const CosNotification::QoSProperties&
TAO_Notify_Properties::default_event_channel_qos_properties (void)
{
  return this->ec_qos_;
}

ACE_INLINE void
TAO_Notify_Properties::default_event_channel_qos_properties (const CosNotification::QoSProperties &ec_qos)
{
  this->ec_qos_ = ec_qos;
}

ACE_INLINE const CosNotification::QoSProperties&
TAO_Notify_Properties::default_supplier_admin_qos_properties (void)
{
  return this->sa_qos_;
}

ACE_INLINE void
TAO_Notify_Properties::default_supplier_admin_qos_properties (const CosNotification::QoSProperties &sa_qos)
{
  this->sa_qos_ = sa_qos;
}

ACE_INLINE const CosNotification::QoSProperties&
TAO_Notify_Properties::default_consumer_admin_qos_properties (void)
{
  return this->ca_qos_;
}

ACE_INLINE void
TAO_Notify_Properties::default_consumer_admin_qos_properties (const CosNotification::QoSProperties &ca_qos)
{
  this->ca_qos_ = ca_qos;
}

ACE_INLINE const CosNotification::QoSProperties&
TAO_Notify_Properties::default_proxy_supplier_qos_properties (void)
{
  return this->ps_qos_;
}

ACE_INLINE void
TAO_Notify_Properties::default_proxy_supplier_qos_properties (const CosNotification::QoSProperties &ps_qos)
{
  this->ps_qos_ = ps_qos;
}

ACE_INLINE const CosNotification::QoSProperties&
TAO_Notify_Properties::default_proxy_consumer_qos_properties (void)
{
  return this->pc_qos_;
}

ACE_INLINE void
TAO_Notify_Properties::default_proxy_consumer_qos_properties (const CosNotification::QoSProperties &pc_qos)
{
  this->pc_qos_ = pc_qos;
}

ACE_INLINE CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_Properties::defaultConsumerAdminFilterOp (void)
{
  return this->defaultConsumerAdminFilterOp_;
}

ACE_INLINE void
TAO_Notify_Properties::defaultConsumerAdminFilterOp (CosNotifyChannelAdmin::InterFilterGroupOperator op)
{
  this->defaultConsumerAdminFilterOp_ = op;
}


ACE_INLINE CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_Properties::defaultSupplierAdminFilterOp (void)
{
  return this->defaultSupplierAdminFilterOp_;
}

ACE_INLINE void
TAO_Notify_Properties::defaultSupplierAdminFilterOp (CosNotifyChannelAdmin::InterFilterGroupOperator op)
{
  this->defaultSupplierAdminFilterOp_ = op;
}

TAO_END_VERSIONED_NAMESPACE_DECL
