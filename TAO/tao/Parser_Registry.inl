// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Parser_Registry::Parser_Iterator
TAO_Parser_Registry::begin () const
{
  return this->parsers_;
}

ACE_INLINE TAO_Parser_Registry::Parser_Iterator
TAO_Parser_Registry::end () const
{
  return this->parsers_ + this->size_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
