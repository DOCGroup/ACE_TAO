// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO_ORB_Core_Auto_Ptr::TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core *p)
  : p_ (p)
{
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Core_Auto_Ptr::get (void) const
{
  return this->p_;
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Core_Auto_Ptr::release (void)
{
  TAO_ORB_Core *old = this->p_;
  this->p_ = 0;
  return old;
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Core_Auto_Ptr::operator-> () const
{
  return this->get ();
}

ACE_INLINE
TAO_ORB_Core_Auto_Ptr::TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core_Auto_Ptr &rhs)
  : p_ (rhs.release ())
{
}

ACE_INLINE TAO_ORB_Core_Auto_Ptr &
TAO_ORB_Core_Auto_Ptr::operator= (TAO_ORB_Core_Auto_Ptr &rhs)
{
  if (this != &rhs)
    {
      this->reset (rhs.release ());
    }
  return *this;
}

// Accessor methods to the underlying ORB_Core Object

ACE_INLINE TAO_ORB_Core &
TAO_ORB_Core_Auto_Ptr::operator *() const
{
  // @@ Potential problem if this->p_ is zero!
  return *this->get ();
}
