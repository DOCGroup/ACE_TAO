// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA::release (CORBA::BOA_ptr obj)
{
  if (obj)
    obj->Release ();
}

