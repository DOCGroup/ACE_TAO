// -*- C++ -*-
//
// $Id$

ACE_INLINE 
CORBA::Boolean
CORBA::is_nil (CORBA::Principal_ptr principal)
{
  return (CORBA::Boolean) (principal == 0);
}

ACE_INLINE 
CORBA::ULong
CORBA::Principal::_decr_refcnt (void)
{
  {
    this->refcount_--;

    if (this->refcount_ != 0)
      {
        return this->refcount_;
      }
  }

  delete this;
  return 0;
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
CORBA::ULong
CORBA::Principal::_incr_refcnt (void)
{
  return this->refcount_++;
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

