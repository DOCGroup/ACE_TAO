// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Parser_Registry::Parser_Iterator
TAO_Parser_Registry::begin () const
{
  return this->parsers_.cbegin ();
}

ACE_INLINE TAO_Parser_Registry::Parser_Iterator
TAO_Parser_Registry::end () const
{
  return this->parsers_.cend ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
