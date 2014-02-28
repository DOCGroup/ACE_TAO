// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ---------------------------------------------------------------------------
//  ORB specific
// ---------------------------------------------------------------------------

ACE_INLINE ACE_Time_Value *
CORBA::ORB::get_timeout (void)
{
  return this->timeout_;
}

ACE_INLINE void
CORBA::ORB::set_timeout (ACE_Time_Value *timeout)
{
  this->timeout_ = timeout;
}

ACE_INLINE unsigned long
CORBA::ORB::_incr_refcount (void)
{
  return ++this->refcount_;
}

ACE_INLINE unsigned long
CORBA::ORB::_refcount (void) const
{
  return this->refcount_.value ();
}

ACE_INLINE unsigned long
CORBA::ORB::_decr_refcount (void)
{
  unsigned long const count = --this->refcount_;

  if (count != 0)
    {
      return count;
    }

  delete this;
  return 0;
}

ACE_INLINE CORBA::ORB_ptr
CORBA::ORB::_duplicate (CORBA::ORB_ptr obj)
{
  if (obj)
    {
      obj->_incr_refcount ();
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

TAO_END_VERSIONED_NAMESPACE_DECL
