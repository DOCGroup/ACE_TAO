// $Id$

ACE_INLINE void
TAO_Sync_Scope_Policy::get_synchronization (Messaging::SyncScope &s) const
{
  s = this->synchronization_;
}
