ACE_INLINE
int
CORBA_BOA::bind(const CORBA_OctetSeq &key, CORBA_Object_ptr obj)
{
  return objtable_->bind(key, obj);
}

ACE_INLINE
int
CORBA_BOA::find(const CORBA_OctetSeq &key, CORBA_Object_ptr &obj)
{
  return objtable_->find(key, obj);
}

