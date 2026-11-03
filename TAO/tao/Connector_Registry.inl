// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_ConnectorSetIterator
TAO_Connector_Registry::begin () const
{
  return this->connectors_.get ();
}

ACE_INLINE TAO_ConnectorSetIterator
TAO_Connector_Registry::end () const
{
  TAO_Connector ** const connectors = this->connectors_.get ();
  return this->size_ == 0 ? connectors : connectors + this->size_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
