// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::Context_ptr
CORBA::Context::_duplicate (CORBA::Context_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcount ();
    }

  return x;
}

ACE_INLINE
CORBA::Context_ptr
CORBA::Context::_nil (void)
{
  return (CORBA::Context_ptr)0;
}

// *************************************************************
// Inline operations for class CORBA::ContextList
// *************************************************************

ACE_INLINE
CORBA::ContextList::ContextList (void)
{
}

ACE_INLINE
CORBA::ULong
CORBA::ContextList::count (void)
{
  return (CORBA::ULong) this->ctx_list_.size ();
}

ACE_INLINE
CORBA::ContextList_ptr
CORBA::ContextList::_nil (void)
{
  return (CORBA::ContextList_ptr)0;
}

ACE_INLINE
CORBA::ContextList_ptr
CORBA::ContextList::_duplicate (CORBA::ContextList_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcount ();
    }

  return x;
}

TAO_END_VERSIONED_NAMESPACE_DECL
