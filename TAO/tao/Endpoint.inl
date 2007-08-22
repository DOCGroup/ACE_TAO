// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Endpoint::TAO_Endpoint (CORBA::ULong tag,
                            CORBA::Short priority)
  : addr_lookup_lock_ ()
  // @@ Would be a tragedy if the hash value of this endpoint is 0, in
  //    which case this optimizaton wouldn't work. We can get around
  //    that using a bool. But we don't want to increase the runtime
  //    memory.
  , hash_val_ (0)
  , tag_ (tag)
  , priority_ (priority)

{
}

ACE_INLINE CORBA::ULong
TAO_Endpoint::tag (void) const
{
  return this->tag_;
}

ACE_INLINE CORBA::Short
TAO_Endpoint::priority (void) const
{
  return this->priority_;
}

ACE_INLINE void
TAO_Endpoint::priority (CORBA::Short p)
{
  this->priority_ = p;
}

TAO_END_VERSIONED_NAMESPACE_DECL
