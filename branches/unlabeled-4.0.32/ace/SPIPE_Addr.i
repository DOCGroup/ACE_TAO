/* -*- C++ -*- */
// $Id$

// SPIPE_Addr.i

// Transform the current address into string format. 

ACE_INLINE int
ACE_SPIPE_Addr::addr_to_string (char s[], size_t len) const
{
  ACE_OS::strncpy (s, this->SPIPE_addr_.rendezvous_, len);
  return 0;
}

// Return the address. 

ACE_INLINE void *
ACE_SPIPE_Addr::get_addr (void) const
{
  return (void *) &this->SPIPE_addr_;
}

// Compare two addresses for equality. 

ACE_INLINE int
ACE_SPIPE_Addr::operator == (const ACE_Addr &sap) const
{
  return ACE_OS::strcmp (this->SPIPE_addr_.rendezvous_,
			 ((ACE_SPIPE_Addr &) sap).SPIPE_addr_.rendezvous_) == 0;
}

// Compare two addresses for inequality. 

ACE_INLINE int
ACE_SPIPE_Addr::operator != (const ACE_Addr &sap) const
{
  return !((*this) == sap);	// This is lazy, of course... ;-)
}

// Return the path name used for the rendezvous point.

ACE_INLINE LPCTSTR
ACE_SPIPE_Addr::get_path_name (void) const
{
  return this->SPIPE_addr_.rendezvous_;
}

ACE_INLINE uid_t
ACE_SPIPE_Addr::user_id (void) const
{
  return this->SPIPE_addr_.uid_;
}

ACE_INLINE void	         
ACE_SPIPE_Addr::user_id (uid_t uid)
{
  this->SPIPE_addr_.uid_ = uid;
}

ACE_INLINE gid_t
ACE_SPIPE_Addr::group_id (void) const
{
  return this->SPIPE_addr_.gid_;
}

ACE_INLINE void
ACE_SPIPE_Addr::group_id (gid_t gid)
{
  this->SPIPE_addr_.gid_ = gid;
}
