// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_Endpoint::TAO_Endpoint (CORBA::ULong tag,
                            CORBA::Short priority)
  : tag_ (tag),
    priority_ (priority)
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
