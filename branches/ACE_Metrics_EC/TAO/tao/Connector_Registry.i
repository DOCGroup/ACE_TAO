// -*- C++ -*-
// $Id$
ACE_INLINE TAO_Connector *
TAO_Connector_Registry::get_connector (TAO_Endpoint *ep)
{
  return this->get_connector (ep->tag ());
}

ACE_INLINE TAO_ConnectorSetIterator
TAO_Connector_Registry::begin (void)
{
  return this->connectors_;
}

ACE_INLINE TAO_ConnectorSetIterator
TAO_Connector_Registry::end (void)
{
  return this->connectors_ + this->size_;
}
