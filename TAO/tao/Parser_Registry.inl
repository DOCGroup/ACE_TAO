// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Parser_Registry::const_iterator
TAO_Parser_Registry::begin () const
{
  return this->parsers_.begin ();
}

ACE_INLINE TAO_Parser_Registry::const_iterator
TAO_Parser_Registry::end () const
{
  return this->parsers_.end ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
