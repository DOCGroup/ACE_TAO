//
// Constructor and destructor are accessible to subclasses
//
ACE_INLINE
CORBA_ORB::CORBA_ORB ()
{
    _refcount = 1;
}

ACE_INLINE
CORBA_ORB::~CORBA_ORB ()
{
    assert (_refcount == 0);
}

//
// CORBA dup/release build on top of COM's (why not).
//
ACE_INLINE
void
CORBA_release (CORBA_ORB_ptr obj)
{
  if (obj)
    obj->Release ();
}

ACE_INLINE
CORBA_ORB_ptr
CORBA_ORB::_duplicate (CORBA_ORB_ptr obj)
{
  if (obj)
    obj->AddRef ();
  return obj;
}

//
// Null pointers represent nil objects.
//
ACE_INLINE
CORBA_ORB_ptr
CORBA_ORB::_nil ()
{
  return 0;
}

ACE_INLINE
CORBA_Boolean
CORBA_is_nil (CORBA_ORB_ptr obj)
{
  return (CORBA_Boolean) (obj == 0);
}

ACE_INLINE
ULONG
__stdcall
CORBA_ORB::AddRef ()
{
  ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, lock_, 0);

  return _refcount++;
}
