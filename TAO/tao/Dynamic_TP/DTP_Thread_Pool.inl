// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_DTP_Thread_Lane &
TAO_DTP_Thread_Pool_Threads::lane (void) const
{
  return this->lane_;
}

ACE_INLINE
CORBA::ULong
TAO_DTP_Thread_Lane::id (void) const
{
  return this->id_;
}

ACE_INLINE
TAO_DTP_Thread_Pool &
TAO_DTP_Thread_Lane::pool (void) const
{
  return this->pool_;
}

ACE_INLINE
CORBA::ULong
TAO_DTP_Thread_Lane::minimum_threads (void) const
{
  return this->minimum_threads_number_;
}

ACE_INLINE
CORBA::ULong
TAO_DTP_Thread_Lane::maximum_threads (void) const
{
  return this->maximum_threads_number_;
}

ACE_INLINE
TAO_Thread_Lane_Resources &
TAO_DTP_Thread_Lane::resources (void)
{
  return this->resources_;
}

ACE_INLINE
ACE_Time_Value const &
TAO_DTP_Thread_Lane::dynamic_thread_time (void) const
{
  return this->dynamic_thread_time_;
}

ACE_INLINE
TAO_DTP_Thread_Pool_Manager &
TAO_DTP_Thread_Pool::manager (void) const
{
  return this->manager_;
}

ACE_INLINE
CORBA::ULong
TAO_DTP_Thread_Pool::id (void) const
{
  return this->id_;
}

ACE_INLINE
CORBA::ULong
TAO_DTP_Thread_Pool::stack_size (void) const
{
  return this->definition_.stack_size_;
}

#if 0
ACE_INLINE
TAO_DTP_Thread_Lane **
TAO_DTP_Thread_Pool::lanes (void)
{
  return this->lanes_;
}
#endif /*0*/

TAO_END_VERSIONED_NAMESPACE_DECL
