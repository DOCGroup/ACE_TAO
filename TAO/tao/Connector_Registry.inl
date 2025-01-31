// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_ConnectorSetIterator
TAO_Connector_Registry::begin () const
{
  return this->connectors_;
}

ACE_INLINE TAO_ConnectorSetIterator
TAO_Connector_Registry::end () const
{
  return this->connectors_ + this->size_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
