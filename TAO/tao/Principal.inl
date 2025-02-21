// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
unsigned long
CORBA::Principal::_decr_refcount ()
{
  unsigned long const new_count = --this->refcount_;

  if (new_count == 0)
    delete this;

  return new_count;
}

ACE_INLINE
unsigned long
CORBA::Principal::_incr_refcount ()
{
  return ++this->refcount_;
}


ACE_INLINE
CORBA::Principal *
CORBA::Principal::_duplicate (CORBA::Principal * x)
{
  if (x)
    {
      x->_incr_refcount ();
    }

  return x;
}


ACE_INLINE
CORBA::Principal *
CORBA::Principal::_nil ()
{
  return nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
