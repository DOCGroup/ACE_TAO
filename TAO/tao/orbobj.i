// $Id$

// Constructor and destructor are accessible to subclasses

ACE_INLINE
CORBA_ORB::~CORBA_ORB (void)
{
  ACE_Service_Config::close();

  if (! client_factory_from_service_config_)
    delete client_factory_;

  if (! server_factory_from_service_config_)
    delete server_factory_;

  // This assertion isn't valid because our ORB is a singleton
  // assert (refcount_ == 0);
}

// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA::release (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->Release ();
}

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_duplicate (CORBA::ORB_ptr obj)
{
  if (obj)
    obj->AddRef ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA::ORB_ptr
CORBA_ORB::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::ORB_ptr obj)
{
  return (CORBA::Boolean) (obj == 0);
}

ACE_INLINE ULONG __stdcall
CORBA_ORB::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));

  return refcount_++;
}

ACE_INLINE
TAO_ORB_Parameters *
CORBA_ORB::params(void)
{
  return &params_;
}

ACE_INLINE CORBA::Boolean
CORBA_ORB::work_pending (void)
{
  // There's ALWAYS work to do ;-)
  return CORBA::B_TRUE;
}

ACE_INLINE void
CORBA_ORB::shutdown (CORBA::Boolean wait_for_completion)
{
  return;
}


