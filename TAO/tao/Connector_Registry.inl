// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Connector_Registry::const_iterator
TAO_Connector_Registry::begin () const
{
  return this->connectors_.begin ();
}

ACE_INLINE TAO_Connector_Registry::const_iterator
TAO_Connector_Registry::end () const
{
  return this->connectors_.end ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
