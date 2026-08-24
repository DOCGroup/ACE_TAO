#include "ace/RAW_Socket.h"

#include "ace/ACE.h"
#include "ace/Log_Category.h"
#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

// Included so users have access to ACE_RECVPKTINFO and ACE_RECVPKTINFO6.
#include "ace/OS_NS_sys_socket.h"

#if defined (ACE_HAS_IPV6) && defined (ACE_WIN32)
#include /**/ <iphlpapi.h>
#endif

#if !defined (__ACE_INLINE__)
#include "ace/RAW_Socket.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE (ACE_RAW_SOCKET)

#if defined (ACE_HAS_4_4BSD_SENDMSG_RECVMSG) || defined ACE_WIN32
#define ACE_USE_MSG_CONTROL
union control_buffer
{
  cmsghdr control_msg_header;
#if defined (IP_RECVDSTADDR)
  u_char padding[ACE_CMSG_SPACE (sizeof (in_addr))];
#elif defined (IP_PKTINFO)
  u_char padding[ACE_CMSG_SPACE (sizeof (in_pktinfo))];
#endif
#if defined (ACE_HAS_IPV6)
  u_char padding6[ACE_CMSG_SPACE (sizeof (in6_pktinfo))];
#endif
};
#endif

#define ACE_SEND_EXCEPTION_RETURN() do {\
  if (this->get_handle () == ACE_INVALID_HANDLE)\
    return -1;\
  if(timeout && ACE::handle_write_ready (this->get_handle (), timeout) != 1)\
    return -1;\
} while(0)

#define ACE_RECV_EXCEPTION_RETURN() do {\
  if (this->get_handle () == ACE_INVALID_HANDLE)\
    return -1;\
  if(this->is_send_only())\
    return -1;\
  if(timeout && ACE::handle_read_ready (this->get_handle (), timeout) != 1)\
    return -1;\
} while(0)

void
ACE_RAW_SOCKET::dump () const
{
  ACE_TRACE ("ACE_RAW_SOCKET::dump");
}

/// @brief default constructor
ACE_RAW_SOCKET::ACE_RAW_SOCKET () : protocol_(IPPROTO_UDP)
{
  ACE_TRACE ("ACE_RAW_SOCKET::ACE_RAW_SOCKET");
}

/// @brief constructor with parameters
ACE_RAW_SOCKET::ACE_RAW_SOCKET (ACE_INET_Addr const & local,
                                int protocol) : protocol_(protocol)
{
  ACE_TRACE ("ACE_RAW_SOCKET::ACE_RAW_SOCKET");

  if (this->open (local, protocol) == -1)
    ACELIB_ERROR ((LM_ERROR,
                   ACE_TEXT ("%p\n"),
                   ACE_TEXT ("ACE_RAW_SOCKET")));
}

static inline ssize_t using_common_recv(const ACE_RAW_SOCKET& raw, void *buf, size_t n, ACE_INET_Addr &addr, int flags)
{
  struct sockaddr *saddr      = static_cast<struct sockaddr *>(addr.get_addr ());
  int addr_len                = addr.get_size ();

  ssize_t const status = ACE_OS::recvfrom (raw.get_handle (),
                                           static_cast<char*>(buf),
                                           n,
                                           flags,
                                           saddr,
                                           &addr_len);

  addr.set_size (addr_len);
  addr.set_type (saddr->sa_family);
  return status;
}

static inline void fillMsgHdr(msghdr& recv_msg, const ACE_INET_Addr &addr, void* pcbuf, int cbuf_size)
{
#if defined (ACE_HAS_SOCKADDR_MSG_NAME)
  recv_msg.msg_name = static_cast<struct sockaddr *> (addr.get_addr ());
#else
  recv_msg.msg_name = static_cast<char *>(addr.get_addr ());
#endif /* ACE_HAS_SOCKADDR_MSG_NAME */

  recv_msg.msg_namelen = addr.get_size ();

#ifdef ACE_USE_MSG_CONTROL
  recv_msg.msg_control      =  pcbuf;
  recv_msg.msg_controllen   =  cbuf_size;
#elif !defined ACE_LACKS_SENDMSG
  recv_msg.msg_accrights    = 0;
  recv_msg.msg_accrightslen = 0;
#endif
}

static inline void getToAddrFromMsgHdr(msghdr& recv_msg, ACE_INET_Addr& to_addr)
{
#ifdef ACE_USE_MSG_CONTROL
  if (to_addr.get_type() == AF_INET)
  {
#if defined (IP_RECVDSTADDR) || defined (IP_PKTINFO)
    for (cmsghdr *ptr = ACE_CMSG_FIRSTHDR (&recv_msg); ptr; ptr = ACE_CMSG_NXTHDR (&recv_msg, ptr))
    {
#if defined (IP_RECVDSTADDR)
      if (ptr->cmsg_level == IPPROTO_IP && ptr->cmsg_type == IP_RECVDSTADDR)
      {
        to_addr.set_address (reinterpret_cast<char *>(ACE_CMSG_DATA (ptr)),
                             sizeof (struct in_addr),
                             0);
        break;
      }
#else
      if (ptr->cmsg_level == IPPROTO_IP && ptr->cmsg_type == IP_PKTINFO)
      {
        to_addr.set_address (reinterpret_cast<const char *>(&((reinterpret_cast<in_pktinfo *>(ACE_CMSG_DATA (ptr)))->ipi_addr)),
                             sizeof (struct in_addr),
                             0);
        break;
      }
#endif
    }
#endif
  }
#if defined (ACE_HAS_IPV6) && defined (IPV6_PKTINFO)
  else if (to_addr.get_type() == AF_INET6)
  {
    for (cmsghdr *ptr = ACE_CMSG_FIRSTHDR (&recv_msg); ptr; ptr = ACE_CMSG_NXTHDR (&recv_msg, ptr))
      {
          if (ptr->cmsg_level == IPPROTO_IPV6 && ptr->cmsg_type == IPV6_PKTINFO)
          {
            to_addr.set_address (reinterpret_cast<const char *>(&((reinterpret_cast<in6_pktinfo *>((ACE_CMSG_DATA(ptr))))->ipi6_addr)),
                                 sizeof (struct in6_addr),
                                 0);
            break;
          }
      }
  }
#endif
#else
  ACE_UNUSED_ARG(recv_msg);
  ACE_UNUSED_ARG (to_addr);
#endif
}

ssize_t
ACE_RAW_SOCKET::recv (void *buf,
                      size_t n,
                      ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout,
                      ACE_INET_Addr *to_addr) const
{
  ACE_TRACE ("ACE_RAW_SOCKET::recv");

  ACE_RECV_EXCEPTION_RETURN();

  if(to_addr == nullptr)
  {
    return using_common_recv(*this, buf, n, addr, flags);
  }
  else
  {
    this->get_local_addr (*to_addr);
    if(!to_addr->is_any())
    {
      return using_common_recv(*this, buf, n, addr, flags);
    }
  }

  struct iovec iov;
  iov.iov_base = static_cast<char*>(buf);
  iov.iov_len  = static_cast<int>(n);

  msghdr recv_msg     = {};
  recv_msg.msg_iov    = &iov;
  recv_msg.msg_iovlen = 1;

#ifdef ACE_USE_MSG_CONTROL
  union control_buffer  cbuf;
  if(to_addr != nullptr)
  {
    fillMsgHdr(recv_msg, addr, &cbuf, sizeof(cbuf));
  }
  else
  {
    fillMsgHdr(recv_msg, addr, nullptr, 0);
  }
#else
  fillMsgHdr(recv_msg, addr, nullptr, 0);
#endif

  ssize_t const status = ACE_OS::recvmsg (this->get_handle (),
                                          &recv_msg,
                                          flags);

  addr.set_size (recv_msg.msg_namelen);
  addr.set_type ((static_cast<sockaddr_in *>(addr.get_addr()))->sin_family);

#ifdef ACE_USE_MSG_CONTROL
  if(to_addr != nullptr)
    getToAddrFromMsgHdr(recv_msg, *to_addr);
#endif

  return status;
}

ssize_t
ACE_RAW_SOCKET::send (const void *buf,
                      size_t n,
                      const ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_RAW_SOCKET::send");

  // Check the status of the current socket.
  ACE_SEND_EXCEPTION_RETURN();

  struct sockaddr *saddr = static_cast<struct sockaddr *>(addr.get_addr ());
  int const len   = addr.get_size ();
  return ACE_OS::sendto (this->get_handle (),
                         static_cast<const char *>(buf),
                         n,
                         flags,
                         saddr,
                         len);
}

#if defined (ACE_HAS_MSG)
ssize_t
ACE_RAW_SOCKET::send (const iovec iov[],
                      int n,
                      const ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_RAW_SOCKET::send");

  // Check the status of the current socket.
  ACE_SEND_EXCEPTION_RETURN();

  msghdr send_msg     = {};
  send_msg.msg_iov    = const_cast<iovec *>(iov);
  send_msg.msg_iovlen = n;

#if defined (ACE_HAS_SOCKADDR_MSG_NAME)
  send_msg.msg_name = static_cast<struct sockaddr *>(addr.get_addr());
#else
  send_msg.msg_name    = static_cast<char*>(addr.get_addr ());
#endif /* ACE_HAS_SOCKADDR_MSG_NAME */
  send_msg.msg_namelen = addr.get_size ();

#if defined (ACE_HAS_4_4BSD_SENDMSG_RECVMSG)
  send_msg.msg_control    = 0;
  send_msg.msg_controllen = 0;
  send_msg.msg_flags      = 0;
#elif !defined ACE_LACKS_SENDMSG
  send_msg.msg_accrights    = 0;
  send_msg.msg_accrightslen = 0;
#endif /* ACE_HAS_4_4BSD_SENDMSG_RECVMSG */

#ifdef ACE_WIN32
  send_msg.msg_control    = 0;
  send_msg.msg_controllen = 0;
#endif

  return ACE_OS::sendmsg (this->get_handle (), &send_msg, flags);
}

ssize_t
ACE_RAW_SOCKET::recv (iovec iov[],
                      int n,
                      ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout,
                      ACE_INET_Addr *to_addr) const
{
  ACE_TRACE ("ACE_RAW_SOCKET::recv");

  ACE_RECV_EXCEPTION_RETURN();

  msghdr recv_msg     = {};
  recv_msg.msg_iov    = static_cast<struct iovec *>(iov);
  recv_msg.msg_iovlen = n;

  /*default*/
  fillMsgHdr(recv_msg, addr, nullptr, 0);

#ifdef ACE_USE_MSG_CONTROL
  union control_buffer  cbuf;
  if(to_addr != nullptr)
  {
    this->get_local_addr(*to_addr);
    fillMsgHdr(recv_msg, addr, &cbuf, sizeof(cbuf));
  }
#endif

  ssize_t const status = ACE_OS::recvmsg (this->get_handle (),
                                          &recv_msg,
                                          flags);

  addr.set_size (recv_msg.msg_namelen);
  addr.set_type ((static_cast<sockaddr_in *>(addr.get_addr()))->sin_family);

#ifdef ACE_USE_MSG_CONTROL
  if(to_addr != nullptr)
    getToAddrFromMsgHdr(recv_msg, *to_addr);
#endif

  return status;
}
#else
// not supported will fail immediately
ssize_t
ACE_RAW_SOCKET::send (const iovec iov[],
                      int n,
                      const ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_RAW_SOCKET::send iovec");

  // Immediately fail when unsupported.
  return -1;
}

ssize_t
ACE_RAW_SOCKET::recv (iovec iov[],
                      int n,
                      ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout,
                      ACE_INET_Addr *to_addr) const
{
  ACE_TRACE ("ACE_RAW_SOCKET::recv iovec");

  // Immediately fail when unsupported.
  return -1;
}
#endif

int
ACE_RAW_SOCKET::open (ACE_INET_Addr const & local, int protocol)
{
  ACE_TRACE ("ACE_RAW_SOCKET::open");

  if (this->get_handle () != ACE_INVALID_HANDLE)
    return -1;

  int const protocol_family  = local.get_type ();
  /// Reuse_addr Maybe meaningless for RAW Socket.
  int const reuse_addr = 1;

  if(ACE_SOCK::open (SOCK_RAW, protocol_family, protocol, reuse_addr) == -1)
    return -1;

  if(ACE_OS::bind (this->get_handle (), static_cast<struct sockaddr *>(local.get_addr()), local.get_addr_size()) == -1)
    return -1;

  this->protocol_ = protocol;

  ACE_INET_Addr bindAddr;
  this->get_local_addr(bindAddr);

#if defined (ACE_HAS_IPV6) && defined (IPV6_PKTINFO) && defined(IPV6_RECVPKTINFO)
  if (bindAddr.get_type() == PF_INET6 && bindAddr.is_any())
  {
    int yes = 1;
    this->set_option(IPPROTO_IPV6, ACE_RECVPKTINFO6, &yes, sizeof(yes));
  }
#endif

#if defined (IP_RECVDSTADDR) || defined (IP_PKTINFO)
  if (bindAddr.get_type() == PF_INET && bindAddr.is_any())
  {
    int yes = 1;
    this->set_option(IPPROTO_IP, ACE_RECVPKTINFO, &yes, sizeof(yes));
  }
#endif

  return  0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
