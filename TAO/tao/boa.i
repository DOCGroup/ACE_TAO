// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA_release (CORBA_BOA_ptr obj)
{
  if (obj)
    obj->Release ();
}

