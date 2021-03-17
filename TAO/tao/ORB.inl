// -*- C++ -*-
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
CORBA::ORB::_incr_refcount ()
{
  return ++this->refcount_;
}

ACE_INLINE unsigned long
CORBA::ORB::_refcount () const
{
  return this->refcount_;
}

ACE_INLINE unsigned long
CORBA::ORB::_decr_refcount ()
{
  unsigned long const count = --this->refcount_;

  if (count == 0)
    {
      delete this;
    }

  return count;
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
CORBA::ORB::_nil ()
{
  return nullptr;
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
CORBA::ORB::orb_core () const
{
  return this->orb_core_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
