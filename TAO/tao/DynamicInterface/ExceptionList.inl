// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::ExceptionList::ExceptionList ()
  : refcount_ (1)
{
}

ACE_INLINE
CORBA::ULong
CORBA::ExceptionList::count ()
{
  return (CORBA::ULong) this->tc_list_.size ();
}

ACE_INLINE
CORBA::ExceptionList_ptr
CORBA::ExceptionList::_nil ()
{
  return nullptr;
}

ACE_INLINE
CORBA::ExceptionList_ptr
CORBA::ExceptionList::_duplicate (CORBA::ExceptionList_ptr x)
{
  if (x)
    {
      x->_incr_refcount ();
    }

  return x;
}

TAO_END_VERSIONED_NAMESPACE_DECL
