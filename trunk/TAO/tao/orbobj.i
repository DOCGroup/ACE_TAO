// $Id$

// Constructor and destructor are accessible to subclasses
ACE_INLINE
CORBA_ORB::CORBA_ORB (void)
{
  refcount_ = 1;
}

ACE_INLINE
CORBA_ORB::~CORBA_ORB (void)
{
  assert (refcount_ == 0);
}

// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA_release (CORBA_ORB_ptr obj)
{
  if (obj)
    obj->Release ();
}

ACE_INLINE CORBA_ORB_ptr
CORBA_ORB::_duplicate (CORBA_ORB_ptr obj)
{
  if (obj)
    obj->AddRef ();
  return obj;
}

// Null pointers represent nil objects.

ACE_INLINE CORBA_ORB_ptr
CORBA_ORB::_nil (void)
{
  return 0;
}

ACE_INLINE CORBA_Boolean
CORBA_is_nil (CORBA_ORB_ptr obj)
{
  return (CORBA_Boolean) (obj == 0);
}

ACE_INLINE ULONG __stdcall
CORBA_ORB::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));

  return refcount_++;
}

ACE_INLINE
TAO_Client_Factory&
CORBA_ORB::client_factory(void)
{
  return client_factory_;
}

ACE_INLINE
TAO_Server_Factory&
CORBA_ORB::server_factory(void)
{
  return server_factory_;
}

ACE_INLINE
TAO_ORB_Parameters&
CORBA_ORB::params(void)
{
  return params_;
}
