// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
unsigned long
CORBA::Principal::_decr_refcount (void)
{
  unsigned long new_count = --this->refcount_;

  if (new_count == 0)
    delete this;

  return new_count;
}

ACE_INLINE
unsigned long
CORBA::Principal::_incr_refcount (void)
{
  return ++this->refcount_;
}


ACE_INLINE
CORBA::Principal *
CORBA::Principal::_duplicate (CORBA::Principal * x)
{
  if (x != 0)
    {
      x->_incr_refcount ();
    }

  return x;
}


ACE_INLINE
CORBA::Principal *
CORBA::Principal::_nil (void)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
