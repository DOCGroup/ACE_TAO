/* -*- C++ -*- */
// $Id$

// SOCK_Dgram.i

// Here's the simple-minded constructor. 

ASYS_INLINE
ACE_SOCK_Dgram::ACE_SOCK_Dgram (void)
{
  ACE_TRACE ("ACE_SOCK_Dgram::ACE_SOCK_Dgram");
}

// <sendto> an N byte datagram to <addr> (connectionless version).

ASYS_INLINE ssize_t
ACE_SOCK_Dgram::send (const void *buf, 
		      size_t n, 
		      const ACE_Addr &addr, 
		      int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram::send");
  sockaddr *saddr = (sockaddr *) addr.get_addr ();
  size_t len = addr.get_size ();
  return ACE_OS::sendto (this->get_handle (), 
			 (const char *) buf, n, flags, 
			 (struct sockaddr *) saddr, len);
}

// <recvfrom> an n byte datagram (connectionless version).

ASYS_INLINE ssize_t
ACE_SOCK_Dgram::recv (void *buf, 
		      size_t n, 
		      ACE_Addr &addr, 
		      int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram::recv");
  sockaddr *saddr = (sockaddr *) addr.get_addr ();
  int addr_len = addr.get_size ();

  ssize_t status = ACE_OS::recvfrom (this->get_handle (), 
				     (char *) buf, n, flags, 
				     (sockaddr *) saddr, &addr_len);
  addr.set_size (addr_len);
  return status;
}
