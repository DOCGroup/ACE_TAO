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
                                int protocol,
                                int reuse_addr) : protocol_(protocol)
{
  ACE_TRACE ("ACE_RAW_SOCKET::ACE_RAW_SOCKET");
  if (this->open (local, protocol, reuse_addr) == -1)
  {
      ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_RAW_SOCKET")));
  }
}

ssize_t
ACE_RAW_SOCKET::recv (void *buf,
                      size_t n,
                      ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout) const
{

  if (this->get_handle () == ACE_INVALID_HANDLE)
  {
      return -1;
  }

  if(timeout && ACE::handle_read_ready (this->get_handle (), timeout) != 1)
  {
      return -1;
  }
  
  sockaddr *saddr = (sockaddr *) addr.get_addr ();
  int addr_len    = addr.get_size ();

  ssize_t const status = ACE_OS::recvfrom (this->get_handle (),
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
ACE_RAW_SOCKET::send (const void *buf,
                      size_t n,
                      const ACE_INET_Addr &addr,
                      int flags,
                      const ACE_Time_Value *timeout) const
{
  // Check the status of the current socket.
  if (this->get_handle () == ACE_INVALID_HANDLE)
  {
      return -1;
  }

  if(is_send_only())
  {
    return -1;
  }
  
  if(timeout && ACE::handle_write_ready (this->get_handle (), timeout) != 1)
  {
      return -1;
  }
  

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
ACE_RAW_SOCKET::open (ACE_INET_Addr const & local, int protocol, int reuse_addr)
{
  ACE_TRACE ("ACE_RAW_SOCKET::open");

  if (this->get_handle () != ACE_INVALID_HANDLE)
  {
      return -1;
  }

  int protocol_family = local.get_type ();
  
  
  if(ACE_SOCK::open (SOCK_RAW, protocol_family, protocol, reuse_addr) == -1)
  {
    return -1;
  }

  this->protocol_ = protocol;

  return  0;
}


ACE_END_VERSIONED_NAMESPACE_DECL

