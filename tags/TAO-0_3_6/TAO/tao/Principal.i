// $Id$

ACE_INLINE CORBA::ULong
CORBA_Principal::_incr_refcnt (void)
{
  return this->refcount_++;
}

ACE_INLINE CORBA::ULong
CORBA_Principal::_decr_refcnt (void)
{
  {
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::Principal_ptr principal)
{
  return (CORBA::Boolean) (principal == 0);
}

ACE_INLINE CORBA_Principal*
CORBA_Principal::_duplicate (CORBA_Principal* x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}


ACE_INLINE CORBA_Principal*
CORBA_Principal::_nil (void)
{
  return 0;
}

