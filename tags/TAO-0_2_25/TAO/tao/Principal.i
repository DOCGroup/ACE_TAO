// $Id$

ACE_INLINE void
CORBA::release (CORBA::Principal_ptr principal)
{
  if (principal)
    principal->_decr_refcnt ();
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

