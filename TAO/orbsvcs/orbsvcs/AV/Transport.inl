// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//------------------------------------------------------------
// TAO_AV_Transport_Item
//------------------------------------------------------------
ACE_INLINE
const ACE_CString &
TAO_AV_Transport_Item::name ()
{
  return this->name_;
}

ACE_INLINE
TAO_AV_Transport_Factory *
TAO_AV_Transport_Item::factory ()
{
  return this->factory_;
}

ACE_INLINE
void
TAO_AV_Transport_Item::factory (TAO_AV_Transport_Factory *factory)
{
  this->factory_ = factory;
}


//------------------------------------------------------------
// TAO_AV_Flow_Protocol_Item
//------------------------------------------------------------
ACE_INLINE
const ACE_CString &
TAO_AV_Flow_Protocol_Item::name ()
{
  return this->name_;
}

ACE_INLINE
TAO_AV_Flow_Protocol_Factory *
TAO_AV_Flow_Protocol_Item::factory ()
{
  return this->factory_;
}

ACE_INLINE
void
TAO_AV_Flow_Protocol_Item::factory (TAO_AV_Flow_Protocol_Factory *factory)
{
  this->factory_ = factory;
}

//------------------------------------------------------------
// TAO_AV_Connector_Registry
//------------------------------------------------------------

ACE_INLINE
TAO_AV_ConnectorSetItor
TAO_AV_Connector_Registry::begin ()
{
  return this->connectors_.begin ();
}

ACE_INLINE
TAO_AV_ConnectorSetItor
TAO_AV_Connector_Registry::end ()
{
  return this->connectors_.end ();
}

//------------------------------------------------------------
// TAO_AV_Acceptor_Registry
//------------------------------------------------------------
ACE_INLINE
TAO_AV_AcceptorSetItor
TAO_AV_Acceptor_Registry::begin ()
{
  return this->acceptors_.begin ();
}

ACE_INLINE
TAO_AV_AcceptorSetItor
TAO_AV_Acceptor_Registry::end ()
{
  return this->acceptors_.end ();
}

//------------------------------------------------------------
// TAO_AV_Acceptor
//------------------------------------------------------------
ACE_INLINE
const char *
TAO_AV_Acceptor::flowname ()
{
  return this->flowname_.c_str ();
}

//------------------------------------------------------------
// TAO_AV_Connector
//------------------------------------------------------------
ACE_INLINE
const char *
TAO_AV_Connector::flowname ()
{
  return this->flowname_.c_str ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
