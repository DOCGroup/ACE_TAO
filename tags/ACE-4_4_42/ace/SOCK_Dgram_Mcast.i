/* -*- C++ -*- */
// $Id$

// SOCK_Dgram_Mcast.i

inline int
ACE_SOCK_Dgram_Mcast::set_option (int option, 
				  char optval) 
{ 
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::set_option");
  return this->ACE_SOCK::set_option (IPPROTO_IP, option, 
				     &optval, sizeof (char));
}

inline ssize_t
ACE_SOCK_Dgram_Mcast::send (const void *buf, 
			    size_t n, int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::send");
  return this->ACE_SOCK_Dgram::send (buf, n, 
				     this->mcast_addr_, flags);
}

inline ssize_t
ACE_SOCK_Dgram_Mcast::send (const iovec iov[], 
			    size_t n, int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast::send");
  return this->ACE_SOCK_Dgram::send (iov, n, 
				     this->mcast_addr_, flags);
}
