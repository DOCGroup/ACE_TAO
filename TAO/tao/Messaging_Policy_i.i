// $Id$

ACE_INLINE Messaging::SyncScope
TAO_Sync_Scope_Policy::synchronization (CORBA::Environment &)
{
  return this->synchronization_;
}

ACE_INLINE Messaging::SyncScope
TAO_Sync_Scope_Policy::synchronization (void)
{
  return this->synchronization_;
}

