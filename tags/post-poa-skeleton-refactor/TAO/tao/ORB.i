// -*- C++ -*-
//
// $Id$


// ---------------------------------------------------------------------------
//  ORB specific
// ---------------------------------------------------------------------------

ACE_INLINE CORBA::ULong
CORBA::ORB::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, 
                    guard, 
                    lock_, 
                    0);
  return ++this->refcount_;
}

ACE_INLINE CORBA::ULong
CORBA::ORB::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, 0);
    this->refcount_--;

    if (this->refcount_ != 0)
      {
        return this->refcount_;
      }
  }

  delete this;
  return 0;
}

ACE_INLINE CORBA::ORB_ptr
CORBA::ORB::_duplicate (CORBA::ORB_ptr obj)
{
  if (obj)
    {
      obj->_incr_refcnt ();
    }

  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA::ORB_ptr
CORBA::ORB::_nil (void)
{
  return 0;
}

ACE_INLINE void
CORBA::ORB::_use_omg_ior_format (CORBA::Boolean ior)
{
  this->use_omg_ior_format_ = ior;
}

ACE_INLINE CORBA::Boolean
CORBA::ORB::_use_omg_ior_format (void)
{
  return this->use_omg_ior_format_;
}

ACE_INLINE TAO_ORB_Core *
CORBA::ORB::orb_core (void) const
{
  return this->orb_core_;
}

// ************************************************************
// These are in CORBA namespace
// ************************************************************

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::ORB_ptr obj)
{
  return obj == CORBA::ORB::_nil ();
}

ACE_INLINE void
CORBA::release (CORBA::ORB_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_decr_refcnt ();
}

