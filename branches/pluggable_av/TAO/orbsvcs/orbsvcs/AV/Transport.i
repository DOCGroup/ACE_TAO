/* -*- C++ -*- */
// $Id$

// Transport.i

//------------------------------------------------------------
// TAO_AV_Core
//------------------------------------------------------------

ACE_INLINE
TAO_ORB_Manager*
TAO_AV_Core::orb_manager (void)
{
  return &this->orb_manager_;
}

ACE_INLINE
int
TAO_AV_Core::stop_run (void)
{
  this->stop_run_ = 1;
  return 0;
}

ACE_INLINE
int
TAO_AV_Core::run (void)
{
  this->stop_run_ = 0;
  this->orb_manager_.activate_poa_manager ();
  while (!this->stop_run_ && this->orb_->work_pending ())
    this->orb_->perform_work ();
  return 0;
}

ACE_INLINE
void
TAO_AV_Core::reactor (ACE_Reactor *r)
{
  this->reactor_ = r;
}

ACE_INLINE
ACE_Reactor *
TAO_AV_Core::reactor (void)
{
  return this->reactor_;
}

ACE_INLINE
TAO_AV_Connector_Registry*
TAO_AV_Core::connector_registry (void)
{
  return this->connector_registry_;
}

ACE_INLINE
TAO_AV_Acceptor_Registry*
TAO_AV_Core::acceptor_registry (void)
{
  return this->acceptor_registry_;
}

ACE_INLINE
TAO_AV_TransportFactorySet *
TAO_AV_Core::transport_factories (void)
{
  return &this->transport_factories_;
}

ACE_INLINE
TAO_AV_Flow_ProtocolFactorySet*
TAO_AV_Core::flow_protocol_factories (void)
{
  return &this->flow_protocol_factories_;
}

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
