// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_DTP_Thread_Pool &
TAO_DTP_Thread_Pool_Threads::pool () const
{
  return this->pool_;
}

ACE_INLINE
TAO_DTP_Thread_Pool_Manager &
TAO_DTP_Thread_Pool::manager () const
{
  return this->manager_;
}

ACE_INLINE
CORBA::ULong
TAO_DTP_Thread_Pool::id () const
{
  return this->id_;
}

ACE_INLINE
bool
TAO_DTP_Thread_Pool::use_timeouts () const
{
  return this->definition_.max_threads_ > -1;
}

ACE_INLINE
const ACE_Time_Value&
TAO_DTP_Thread_Pool::dynamic_thread_time () const
{
  return this->definition_.timeout_;
}


TAO_END_VERSIONED_NAMESPACE_DECL
