// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Notify_Object::ID
TAO_Notify_ID_Factory::id (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mtx_, 0);
  return ++seed_;
}

ACE_INLINE void
TAO_Notify_ID_Factory::set_last_used (const TAO_Notify_Object::ID id)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mtx_);
  if (this->seed_ < id)
  {
    this->seed_ = id;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
