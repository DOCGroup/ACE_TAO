// $Id$

ACE_INLINE CORBA::Float
ReplicaProxy_Impl::current_load (void) const
{
  return this->current_load_;
}

ACE_INLINE CORBA::Object_ptr
ReplicaProxy_Impl::replica (void)
{
  return CORBA::Object::_duplicate (this->replica_.in ());
}
