// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Thread_Lane &
TAO_Thread_Pool_Threads::lane () const
{
  return this->lane_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Lane::id () const
{
  return this->id_;
}

ACE_INLINE
TAO_Thread_Pool &
TAO_Thread_Lane::pool () const
{
  return this->pool_;
}

ACE_INLINE
CORBA::Short
TAO_Thread_Lane::lane_priority () const
{
  return this->lane_priority_;
}

ACE_INLINE
CORBA::Short
TAO_Thread_Lane::native_priority () const
{
  return this->native_priority_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Lane::static_threads () const
{
  return this->static_threads_number_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Lane::dynamic_threads () const
{
  return this->dynamic_threads_number_;
}

ACE_INLINE
TAO_Thread_Lane_Resources &
TAO_Thread_Lane::resources ()
{
  return this->resources_;
}

ACE_INLINE
TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan
TAO_Thread_Lane::lifespan () const
{
  return this->lifespan_;
}

ACE_INLINE
ACE_Time_Value const &
TAO_Thread_Lane::dynamic_thread_time () const
{
  return this->dynamic_thread_time_;
}

ACE_INLINE
bool
TAO_Thread_Pool::with_lanes () const
{
  return this->with_lanes_;
}

ACE_INLINE
TAO_Thread_Pool_Manager &
TAO_Thread_Pool::manager () const
{
  return this->manager_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Pool::id () const
{
  return this->id_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Pool::stack_size () const
{
  return this->stack_size_;
}

ACE_INLINE
CORBA::Boolean
TAO_Thread_Pool::allow_borrowing () const
{
  return this->allow_borrowing_;
}

ACE_INLINE
CORBA::Boolean
TAO_Thread_Pool::allow_request_buffering () const
{
  return this->allow_request_buffering_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Pool::max_buffered_requests () const
{
  return this->max_buffered_requests_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Pool::max_request_buffer_size () const
{
  return this->max_request_buffer_size_;
}

ACE_INLINE
TAO_Thread_Lane **
TAO_Thread_Pool::lanes ()
{
  return this->lanes_;
}

ACE_INLINE
CORBA::ULong
TAO_Thread_Pool::number_of_lanes () const
{
  return this->number_of_lanes_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
