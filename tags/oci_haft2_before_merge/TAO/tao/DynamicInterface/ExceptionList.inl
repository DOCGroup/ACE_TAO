// -*- C++ -*-
//
// $Id$

ACE_INLINE
void
CORBA::release (CORBA::ExceptionList_ptr x)
{
  if (x != 0)
    {
      x->_decr_refcnt ();
    }
}

ACE_INLINE
CORBA::Boolean
CORBA::is_nil (CORBA::ExceptionList_ptr x)
{
  return (CORBA::Boolean) (x == 0);
}

// ===================================================================

ACE_INLINE
CORBA::ExceptionList::ExceptionList (void)
  : ref_count_ (1)
{
}

ACE_INLINE 
CORBA::ULong
CORBA::ExceptionList::count (void)
{
  return (CORBA::ULong) this->tc_list_.size ();
}

ACE_INLINE 
CORBA::ExceptionList_ptr
CORBA::ExceptionList::_nil (void)
{
  return (CORBA::ExceptionList_ptr)0;
}

ACE_INLINE 
CORBA::ExceptionList_ptr
CORBA::ExceptionList::_duplicate (CORBA::ExceptionList_ptr x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

