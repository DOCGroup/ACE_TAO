// -*- C++ -*-

ACE_INLINE CORBA::Float
TAO_LB_RPMS_Monitor_Interceptor::current_load (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  ACE_Time_Value elapsed_time =
    ACE_OS::gettimeofday () - this->interval_start_;

  this->interval_start_ = ACE_OS::gettimeofday ();

  CORBA::Float load =
    ACE_static_cast (CORBA::Float,
                     this->request_count_) / elapsed_time.msec ();

  this->request_count_ = 0;
  this->interval_start_ = ACE_OS::gettimeofday ();

  return load;
}

ACE_INLINE void
TAO_LB_RPMS_Monitor_Interceptor::register_redirect (
    const char *type_id,
    CORBA::Object_ptr redirect_to,
    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  this->redirect_table_.register_redirect (type_id,
                                           redirect_to,
                                           ACE_TRY_ENV);
}

ACE_INLINE void
TAO_LB_RPMS_Monitor_Interceptor::remove_redirect (
    const char *type_id,
    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  this->redirect_table_.remove_redirect (type_id,
                                         ACE_TRY_ENV);
}
