// -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE
TAO_Leader_Follower::TAO_Leader_Follower (TAO_ORB_Core* orb_core)
  : orb_core_ (orb_core),
    reverse_lock_ (lock_),
    leaders_ (0),
    clients_ (0),
    reactor_ (0)
{
}

ACE_INLINE TAO_ORB_Core_TSS_Resources *
TAO_Leader_Follower::get_tss_resources (void) const
{
  return this->orb_core_->get_tss_resources ();
}

ACE_INLINE void
TAO_Leader_Follower::set_server_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  tss->is_server_thread_ = 1;

  ++this->leaders_;
}

ACE_INLINE void
TAO_Leader_Follower::reset_server_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  tss->is_server_thread_ = 0;

  --this->leaders_;
}

ACE_INLINE int
TAO_Leader_Follower::leader_available (void) const
{
  return this->leaders_ != 0;
}

ACE_INLINE void
TAO_Leader_Follower::set_client_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->is_server_thread_)
    {
      --this->leaders_;
    }

  if (this->clients_ == 0
      && this->orb_core_->has_shutdown ())
    {
      // The ORB has shutdown and we are the first client after
      // that. This means that the reactor is disabled, we must
      // re-enable it if we want to receive any replys...
      this->orb_core_->reactor ()->reset_reactor_event_loop ();
    }
  this->clients_++;
}

ACE_INLINE void
TAO_Leader_Follower::reset_client_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->is_server_thread_)
    {
      ++this->leaders_;
    }
  this->clients_--;
  if (this->clients_ == 0 && this->orb_core_->has_shutdown ())
    {
      // The ORB has shutdown and we are the last client thread, we
      // must stop the reactor to ensure that any server threads go
      // away.
      this->orb_core_->reactor ()->end_reactor_event_loop ();
    }
}

ACE_INLINE void
TAO_Leader_Follower::set_leader_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->is_leader_thread_ == 0)
    {
      ++this->leaders_;
    }
  ++tss->is_leader_thread_;
}

ACE_INLINE void
TAO_Leader_Follower::reset_leader_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  --tss->is_leader_thread_;
  if (tss->is_leader_thread_ == 0)
    {
      --this->leaders_;
    }
}

ACE_INLINE int
TAO_Leader_Follower::is_leader_thread (void) const
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  return tss->is_leader_thread_ != 0;
}

ACE_INLINE int
TAO_Leader_Follower::follower_available (void) const
{
  return !this->follower_set_.is_empty ();
}

ACE_INLINE int
TAO_Leader_Follower::elect_new_leader (void)
{
  if (this->leaders_ == 0 && this->follower_available ())
    {
      ACE_SYNCH_CONDITION* condition_ptr = this->get_next_follower ();
      if (condition_ptr == 0 || condition_ptr->signal () == -1)
        return -1;
    }
  return 0;
}

ACE_INLINE int
TAO_Leader_Follower::add_follower (ACE_SYNCH_CONDITION *follower_ptr)
{
  return this->follower_set_.insert (follower_ptr);
}

ACE_INLINE int
TAO_Leader_Follower::remove_follower (ACE_SYNCH_CONDITION *follower_ptr)
{
  return this->follower_set_.remove (follower_ptr);
}

ACE_INLINE ACE_SYNCH_MUTEX &
TAO_Leader_Follower::lock (void)
{
  return this->lock_;
}

ACE_INLINE ACE_Reverse_Lock<ACE_SYNCH_MUTEX> &
TAO_Leader_Follower::reverse_lock (void)
{
  return this->reverse_lock_;
}

ACE_INLINE int
TAO_Leader_Follower::has_clients (void) const
{
  return this->clients_;
}
