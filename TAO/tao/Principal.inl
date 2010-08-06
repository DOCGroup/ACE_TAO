// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::Boolean
CORBA::is_nil (CORBA::Principal_ptr principal)
{
  return principal == 0;
}

ACE_INLINE
unsigned long
CORBA::Principal::_decr_refcnt (void)
{
  unsigned long new_count = --this->refcount_;

  if (new_count == 0)
    delete this;

  return new_count;
}

ACE_INLINE
void
CORBA::release (CORBA::Principal_ptr principal)
{
  if (principal)
    {
      principal->_decr_refcnt ();
    }
}

ACE_INLINE
unsigned long
CORBA::Principal::_incr_refcnt (void)
{
  return ++this->refcount_;
}


ACE_INLINE
CORBA::Principal *
CORBA::Principal::_duplicate (CORBA::Principal * x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
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
