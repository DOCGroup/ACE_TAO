// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Boolean
TAO_LB_LoadAlert::alerted (void) const
{
  return this->alerted_;
}

ACE_INLINE CORBA::Object_ptr
TAO_LB_LoadAlert::forward (void)
{
  return CORBA::Object::_duplicate (this->forward_.in ());
}
