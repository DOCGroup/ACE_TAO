// -*- C++ -*-
// $Id$

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
