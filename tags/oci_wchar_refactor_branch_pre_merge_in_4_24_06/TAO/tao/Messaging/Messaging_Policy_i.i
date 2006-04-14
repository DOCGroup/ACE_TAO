// $Id$

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

ACE_INLINE void
TAO_Sync_Scope_Policy::get_synchronization (Messaging::SyncScope &s) const
{
  s = this->synchronization_;
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */
