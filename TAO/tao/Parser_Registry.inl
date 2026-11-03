// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Parser_Registry::Parser_Iterator
TAO_Parser_Registry::begin () const
{
  return this->parsers_.get ();
}

ACE_INLINE TAO_Parser_Registry::Parser_Iterator
TAO_Parser_Registry::end () const
{
  TAO_IOR_Parser ** const parsers = this->parsers_.get ();
  return this->size_ == 0 ? parsers : parsers + this->size_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
