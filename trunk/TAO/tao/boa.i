ACE_INLINE int
CORBA_BOA::bind (const CORBA_OctetSeq &key, 
		 CORBA_Object_ptr obj)
{
  return objtable_->bind (key, obj);
}

ACE_INLINE int
CORBA_BOA::find (const CORBA_OctetSeq &key,
		 CORBA_Object_ptr &obj)
{
  return objtable_->find (key, obj);
}

// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA_release (CORBA_BOA_ptr obj)
{
  if (obj)
    obj->Release ();
}

