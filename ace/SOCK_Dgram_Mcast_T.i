/* -*- C++ -*- */
/* $Id$ */

template <class ACE_SDM_LOCK> ASYS_INLINE int 
ACE_SOCK_Dgram_Mcast_Ex<ACE_SDM_LOCK>::set_option (int option,
                                                   char optval)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::set_option");
#if defined (ACE_WIN32)
  int sock_opt = optval;
  return this->ACE_SOCK::set_option (IPPROTO_IP,
                                     option,
                                     &sock_opt,
                                     sizeof (sock_opt));
#else
  return this->ACE_SOCK::set_option (IPPROTO_IP,
                                     option,
                                     &optval,
                                     sizeof (optval));
#endif /* !ACE_WIN32 */
}

template <class ACE_SDM_LOCK> ASYS_INLINE ssize_t 
ACE_SOCK_Dgram_Mcast_Ex<ACE_SDM_LOCK>::send (const void *buf,
                                             size_t n,
                                             int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::send");
  return this->ACE_SOCK_Dgram::send (buf,
                                     n,
                                     this->send_addr_,
                                     flags);
}

template <class ACE_SDM_LOCK> ASYS_INLINE ssize_t 
ACE_SOCK_Dgram_Mcast_Ex<ACE_SDM_LOCK>::send (const iovec iov[],
                                             size_t n,
                                             int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::send");
  return this->ACE_SOCK_Dgram::send (iov,
                                     n,
                                     this->send_addr_,
                                     flags);
}

template <class ACE_SDM_LOCK> ASYS_INLINE ssize_t 
ACE_SOCK_Dgram_Mcast_Ex<ACE_SDM_LOCK>::send (const void *buf,
                                             size_t n,
                                             const ACE_Addr &addr,
                                             int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::send");
  return this->ACE_SOCK_Dgram::send (buf,
                                     n,
                                     addr,
                                     flags);
}

template <class ACE_SDM_LOCK> ASYS_INLINE ssize_t 
ACE_SOCK_Dgram_Mcast_Ex<ACE_SDM_LOCK>::send (const iovec iov[],
                                             size_t n,
                                             const ACE_Addr &addr,
                                             int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram_Mcast_Ex::send");
  return this->ACE_SOCK_Dgram::send (iov,
                                     n,
                                     addr,
                                     flags);
}
