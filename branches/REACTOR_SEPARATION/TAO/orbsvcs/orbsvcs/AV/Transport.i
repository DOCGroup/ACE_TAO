/* -*- C++ -*- */
// $Id$

// Transport.i



//------------------------------------------------------------
// TAO_AV_Transport_Item
//------------------------------------------------------------
ACE_INLINE
const ACE_CString &
TAO_AV_Transport_Item::name (void)
{
  return this->name_;
}

ACE_INLINE
TAO_AV_Transport_Factory *
TAO_AV_Transport_Item::factory (void)
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
TAO_AV_Flow_Protocol_Item::name (void)
{
  return this->name_;
}

ACE_INLINE
TAO_AV_Flow_Protocol_Factory *
TAO_AV_Flow_Protocol_Item::factory (void)
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
TAO_AV_Connector_Registry::begin (void)
{
  return this->connectors_.begin ();
}

ACE_INLINE
TAO_AV_ConnectorSetItor
TAO_AV_Connector_Registry::end (void)
{
  return this->connectors_.end ();
}

//------------------------------------------------------------
// TAO_AV_Acceptor_Registry
//------------------------------------------------------------
ACE_INLINE
TAO_AV_AcceptorSetItor
TAO_AV_Acceptor_Registry::begin (void)
{
  return this->acceptors_.begin ();
}

ACE_INLINE
TAO_AV_AcceptorSetItor
TAO_AV_Acceptor_Registry::end (void)
{
  return this->acceptors_.end ();
}

//------------------------------------------------------------
// TAO_AV_Acceptor
//------------------------------------------------------------
ACE_INLINE
const char *
TAO_AV_Acceptor::flowname (void)
{
  return this->flowname_.c_str ();
}

//------------------------------------------------------------
// TAO_AV_Connector
//------------------------------------------------------------
ACE_INLINE
const char *
TAO_AV_Connector::flowname (void)
{
  return this->flowname_.c_str ();
}
