// $Id$

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

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

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */
