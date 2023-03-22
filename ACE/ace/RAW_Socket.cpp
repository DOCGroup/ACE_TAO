#include "ace/RAW_Socket.h"

#if !defined (__ACE_INLINE__)
#include "ace/RAW_Socket.inl"
#endif /* __ACE_INLINE__ */

#include "ace/ACE.h"
#include "ace/Log_Category.h"
#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE (ACE_RAW_SOCKET)


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

static inline ssize_t using_common_recv(const ACE_RAW_SOCKET* raw, void *buf, size_t n, ACE_INET_Addr &addr, int flags)
{
    sockaddr *saddr      = (sockaddr *) addr.get_addr ();
    int addr_len         = addr.get_size ();

    ssize_t const status = ACE_OS::recvfrom (raw->get_handle (),
                                      (char *) buf,
                                      n,
                                      flags,
                                      (sockaddr *) saddr,
                                      &addr_len);

    addr.set_size (addr_len);
    addr.set_type (saddr->sa_family);
    return status;
}

ssize_t
ACE_RAW_SOCKET::recv (void *buf,
                      size_t n,
                      ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout,
                      ACE_INET_Addr *to_addr) const
{

  if (this->get_handle () == ACE_INVALID_HANDLE)
      return -1;

  if(this->is_send_only())
    return -1;
  
  if(timeout && ACE::handle_read_ready (this->get_handle (), timeout) != 1)
      return -1;


  if(to_addr == NULL)
  {
    return using_common_recv(this, buf, n, addr, flags);
  }
  else
  {
    this->get_local_addr (*to_addr);
    if(!to_addr->is_any())
    {
       return using_common_recv(this, buf, n, addr, flags);
    }

  }


  #if defined (ACE_HAS_4_4BSD_SENDMSG_RECVMSG) || defined ACE_WIN32
    #define ACE_USE_MSG_CONTROL
    union control_buffer {
      cmsghdr control_msg_header;
      #if defined (IP_RECVDSTADDR)
      u_char padding[ACE_CMSG_SPACE (sizeof (in_addr))];
      #elif defined (IP_PKTINFO)
      u_char padding[ACE_CMSG_SPACE (sizeof (in_pktinfo))];
      #endif
      #if defined (ACE_HAS_IPV6)
      u_char padding6[ACE_CMSG_SPACE (sizeof (in6_pktinfo))];
      #endif
    } cbuf; 
  #endif

  struct iovec iov;
  iov.iov_base = buf;
  iov.iov_len  = n;

  msghdr recv_msg     = {};
  recv_msg.msg_iov    = (iovec*)&iov;
  recv_msg.msg_iovlen = 1;

  #if defined (ACE_HAS_SOCKADDR_MSG_NAME)
    recv_msg.msg_name = (struct sockaddr *) addr.get_addr ();
  #else
    recv_msg.msg_name = (char *) addr.get_addr ();
  #endif /* ACE_HAS_SOCKADDR_MSG_NAME */
    recv_msg.msg_namelen = addr.get_size ();

  #ifdef ACE_USE_MSG_CONTROL
    recv_msg.msg_control    =  &cbuf;
    recv_msg.msg_controllen =  sizeof (cbuf);
  #elif !defined ACE_LACKS_SENDMSG
    recv_msg.msg_accrights    = 0;
    recv_msg.msg_accrightslen = 0;
  #endif

  ssize_t status = ACE_OS::recvmsg (this->get_handle (),
                                      &recv_msg,
                                      flags);

  addr.set_size (recv_msg.msg_namelen);
  addr.set_type (((sockaddr_in *) addr.get_addr())->sin_family);


  #ifdef ACE_USE_MSG_CONTROL
      if (to_addr->get_type() == AF_INET) {
  #if defined (IP_RECVDSTADDR) || defined (IP_PKTINFO)
        for (cmsghdr *ptr = ACE_CMSG_FIRSTHDR (&recv_msg); ptr; ptr = ACE_CMSG_NXTHDR (&recv_msg, ptr)) {
  #if defined (IP_RECVDSTADDR)
          if (ptr->cmsg_level == IPPROTO_IP && ptr->cmsg_type == IP_RECVDSTADDR) {
            to_addr->set_address ((const char *) (ACE_CMSG_DATA (ptr)),
                                  sizeof (struct in_addr),
                                  0);
            break;
          }
  #else
          if (ptr->cmsg_level == IPPROTO_IP && ptr->cmsg_type == IP_PKTINFO) {
            to_addr->set_address ((const char *) &(((in_pktinfo *) (ACE_CMSG_DATA (ptr)))->ipi_addr),
                                  sizeof (struct in_addr),
                                  0);
            break;
          }
  #endif
        }
  #endif
      }
  #if defined (ACE_HAS_IPV6) && defined (IPV6_PKTINFO)
      else if (to_addr->get_type() == AF_INET6) {
        for (cmsghdr *ptr = ACE_CMSG_FIRSTHDR (&recv_msg); ptr; ptr = ACE_CMSG_NXTHDR (&recv_msg, ptr)) {
          if (ptr->cmsg_level == IPPROTO_IPV6 && ptr->cmsg_type == IPV6_PKTINFO) {
            to_addr->set_address ((const char *) &(((in6_pktinfo *)(ACE_CMSG_DATA (ptr)))->ipi6_addr),
                                  sizeof (struct in6_addr),
                                  0);

            break;
          }
        }
      }
  #endif
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
  // Check the status of the current socket.
  if (this->get_handle () == ACE_INVALID_HANDLE)
      return -1;
  
  if(timeout && ACE::handle_write_ready (this->get_handle (), timeout) != 1)
      return -1;
  
  sockaddr *saddr = (sockaddr *) addr.get_addr ();
  int len         = addr.get_size ();
  return ACE_OS::sendto (this->get_handle (),
                    (const char *) buf,
                    n,
                    flags,
                    (struct sockaddr *) saddr,
                    len);
  
}

int
ACE_RAW_SOCKET::open (ACE_INET_Addr const & local, int protocol)
{
  ACE_TRACE ("ACE_RAW_SOCKET::open");

  if (this->get_handle () != ACE_INVALID_HANDLE)
        return -1;

  int protocol_family = local.get_type ();
  /// reuse_addr Maybe meaningless for RAW Socket
  const int reuse_addr = 1;

  if(ACE_SOCK::open (SOCK_RAW, protocol_family, protocol, reuse_addr) == -1)
    return -1;

  this->protocol_ = protocol;
 
  ACE_INET_Addr bindAddr;
  this->get_local_addr(bindAddr);
  if(bindAddr.is_any())
  {
    int yes = 1;
    this->set_option(IPPROTO_IPV6, IPV6_RECVPKTINFO, &yes, sizeof(yes));
  }
  
  return  0;
}


ACE_END_VERSIONED_NAMESPACE_DECL

