/* -*- C++ -*- */
// $Id$


ACE_INLINE void 
ACE_DEV_Addr::set (LPCTSTR devname)
{
  ACE_TRACE ("ACE_DEV_Addr::set");

  this->ACE_Addr::base_set (AF_DEV, ACE_OS::strlen (devname));
  ACE_OS::strncpy (this->devname_, devname, MAXNAMLEN);
}

// Transform the current address into string format.

ACE_INLINE int
ACE_DEV_Addr::addr_to_string (char s[], size_t len) const
{
  ACE_TRACE ("ACE_DEV_Addr::addr_to_string");

  ACE_OS::strncpy (s, this->devname_, len);
  return 0;
}

// Return a pointer to the address. 

ACE_INLINE void *
ACE_DEV_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_DEV_Addr::get_addr");

  return (void *) &this->devname_;
}

// Compare two addresses for equality. 

ACE_INLINE int
ACE_DEV_Addr::operator == (const ACE_Addr &sap) const
{
  ACE_TRACE ("ACE_DEV_Addr::operator=");

  return ACE_OS::strcmp (this->devname_,
			 ((ACE_DEV_Addr &) sap).devname_) == 0;
}

// Compare two addresses for inequality.

ACE_INLINE int
ACE_DEV_Addr::operator != (const ACE_Addr &sap) const
{
  ACE_TRACE ("ACE_DEV_Addr::operator!=");

  return !((*this) == sap);	// This is lazy, of course... ;-).
}

// Return the path name used for the rendezvous point.

ACE_INLINE LPCTSTR
ACE_DEV_Addr::get_path_name (void) const
{
  ACE_TRACE ("ACE_DEV_Addr::get_path_name");

  return this->devname_;
}

