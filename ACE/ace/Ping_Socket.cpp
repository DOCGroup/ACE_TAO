#include "ace/Ping_Socket.h"

#if defined (ACE_HAS_ICMP_SUPPORT) && (ACE_HAS_ICMP_SUPPORT == 1)

#include "ace/INET_Addr.h"
#include "ace/Log_Category.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_unistd.h"
#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

#include <cstring>

#if !defined (__ACE_INLINE__)
# include "ace/Ping_Socket.inl"
#endif  /* !__ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE (ACE_Ping_Socket)

ACE_END_VERSIONED_NAMESPACE_DECL

//---------------------------------------------------------------------------
// Better to arrange some os_include/netinet/ip.h and
// os_include/netinet/icmp.h files ?
//---------------------------------------------------------------------------

#if !defined (ACE_WIN32)

/*
 * This is where ICMP-related stuff is defined on any sane system...
 */
#include /**/ <netinet/in_systm.h>
#include /**/ <netinet/ip.h>
#include /**/ <netinet/ip_icmp.h>

#else  /* #if ! defined (ACE_WIN32) */

/*
 * This was a surprise to me...  This stuff is not defined anywhere under MSVC.
 * These values have only been checked for NT4 and Win2K.  They were taken from
 * the MSDN ping.c program and modified.
 */

#define ICMP_ECHO       8
#define ICMP_ECHOREPLY  0

#pragma pack(1)

struct ip
{
  ACE_UINT8   ip_hl:4;            // length of the header
  ACE_UINT8   version:4;          // Version of IP
  ACE_UINT8   tos;                // Type of service
  ACE_UINT16  total_len;          // total length of the packet
  ACE_UINT16  ident;              // unique identifier
  ACE_UINT16  frag_and_flags;     // flags
  ACE_UINT8   ip_ttl;             // Time to live
  ACE_UINT8   proto;              // protocol (TCP, UDP etc)
  ACE_UINT16  checksum;           // IP checksum
  ACE_UINT32  sourceIP;
  ACE_UINT32  destIP;
};

struct icmp
{
  ACE_UINT8  icmp_type;
  ACE_UINT8  icmp_code;      // type sub code
  ACE_UINT16 icmp_cksum;
  ACE_UINT16 icmp_id;
  ACE_UINT16 icmp_seq;
  ACE_UINT32 icmp_data;      // time data
};

#pragma pack()

#endif /* #if ! defined (ACE_WIN32) */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

int const ICMP_MIN = 8;  // Minimal size of ICMP packet, header only
int const ICMP_DATA_LENGTH = 56;  // For ICMP data with Echo request
ACE_Time_Value const ACE_Ping_Socket::time_default_ (0, 500000);

void
ACE_Ping_Socket::dump () const
{
  ACE_TRACE ("ACE_Ping_Socket::dump");
}

ACE_Ping_Socket::ACE_Ping_Socket ()
{
  ACE_TRACE ("ACE_Ping_Socket::ACE_Ping_Socket");
}

ACE_Ping_Socket::ACE_Ping_Socket (ACE_Addr const & local,
                                  int protocol,
                                  int reuse_addr)
  : sequence_number_ (0),
    connected_socket_ (false)
{
  ACE_TRACE ("ACE_Ping_Socket::ACE_Ping_Socket");

  std::memset (icmp_send_buff_, 0, sizeof (icmp_send_buff_));
  std::memset (icmp_recv_buff_, 0, sizeof (icmp_recv_buff_));

  if (this->open (local, protocol, reuse_addr) == -1)
    {
      ACELIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("ACE_Ping_Socket::ACE_Ping_Socket: %p\n"),
                  ACE_TEXT ("open")));
      return;
    }

  // trying to increase the size of socket receive buffer - some
  // protection from multiple responses e.g., when falling to the
  // multi-cast address
  int size {64 * 1024};
  ACE_SOCK::set_option (SOL_SOCKET,
                        SO_RCVBUF,
                        static_cast<void *> (&size),
                        sizeof (size));
}

ACE_Ping_Socket::~ACE_Ping_Socket ()
{
  ACE_TRACE ("ACE_Ping_Socket::~ACE_Ping_Socket");
}

int
ACE_Ping_Socket::open (ACE_Addr const & local,
                       int protocol,
                       int reuse_addr)
{
  ACE_TRACE ("ACE_Ping_Socket::open");
  return inherited::open (local, protocol, reuse_addr);
}

int
ACE_Ping_Socket::receive_echo_reply (ACE_Time_Value const * timeout)
{
  ACE_TRACE ("ACE_Ping_Socket::receive_echo_reply");

  ACE_Time_Value const before {ACE_OS::gettimeofday ()};
  ACE_Time_Value after;
  ACE_Time_Value time_left;
  ACE_Time_Value *wait_time {const_cast<ACE_Time_Value *> (timeout)};
  ACE_Time_Value const half_millisec (0, 500);

  std::memset (icmp_recv_buff_, 0, sizeof icmp_recv_buff_);

  do
    {
      ssize_t const rval_recv = inherited::recv (icmp_recv_buff_,
                                                 sizeof icmp_recv_buff_,
                                                 0,
                                                 wait_time);
      if (rval_recv < 0)
        {
          if (errno == EINTR)
            {
              after = ACE_OS::gettimeofday ();
              time_left = *timeout - after + before;

              // If more than .5 ms left, wait on select()
              if (time_left > half_millisec)
                {
                  wait_time = &time_left; // coming back to wait on select()
                  continue;
                }
              else
                {
                  break;
                }
            }
          return -1;
        }
      else if (!this->process_incoming_dgram (icmp_recv_buff_, rval_recv))
        {
          return 0; //= success
        }
      else
        {
          after = ACE_OS::gettimeofday ();
          if ((after - before) >= *timeout)
            {
              errno = ETIMEDOUT;
              break;
            }
          // new timeout, we are coming back to sit on select
          *wait_time = *timeout - after + before;
        }
    } while (*wait_time >= half_millisec);

  errno = ETIMEDOUT;
  return -1;
}

int
ACE_Ping_Socket::process_incoming_dgram (char * ptr, ssize_t len)
{
  unsigned char hlen1 {static_cast<unsigned char> (*ptr)};
  struct ip const *ip {reinterpret_cast<struct ip const *> (ptr)};

  // Warning... using knowledge of IP header layout. This avoids a maze of
  // #if blocks for various systems. The first byte of the header has the
  // IP version in the left-most 4 bits and the length in the other 4 bits.
  hlen1 <<= 4;                            // Bump the version off
  hlen1 >>= 4;                            // Zero-extended length remains
  hlen1 <<= 2;                            // Now it counts bytes, not words

  struct icmp const *icmp {reinterpret_cast<struct icmp const *> (ptr + hlen1)};
  ssize_t const icmplen {len - hlen1};

  if (icmplen < ICMP_MIN)
    {
      ACELIB_DEBUG
        ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) ACE_Ping_Socket::process_incoming_dgram")
          ACE_TEXT (" - ICMP length is %b < 8.\n"),
          icmplen));
      ACELIB_ERROR_RETURN
        ((LM_ERROR,
          ACE_TEXT ("(%P|%t) ACE_Ping_Socket::process_incoming_dgram - ")
          ACE_TEXT ("The ICMP header either not received or is corrupted.")),
         -1);
    }

  if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
      ACELIB_DEBUG
        ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) ACE_Ping_Socket::process_incoming_dgram")
          ACE_TEXT (" - ICMP_ECHOREPLY received.\n")));

      if (icmp->icmp_id != (ACE_OS::getpid () & 0xFFFF))
        {
          ACELIB_ERROR_RETURN
            ((LM_ERROR,
              ACE_TEXT ("(%P|%t) ACE_Ping_Socket::")
              ACE_TEXT ("process_incoming_dgram ")
              ACE_TEXT ("- The ICMP header received is a reply to request ")
              ACE_TEXT ("of another process (%d; expected %d).\n"),
              icmp->icmp_id, ACE_OS::getpid()),
             -1);
        }
      if (icmplen < 16)
        {
          ACELIB_ERROR_RETURN
            ((LM_ERROR,
              ACE_TEXT ("(%P|%t) ACE_Ping_Socket::")
              ACE_TEXT ("process_incoming_dgram - ICMP length ")
              ACE_TEXT ("is %b < 16."),
              icmplen),
             -1);
        }

      ACELIB_DEBUG
        ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) ACE::Ping_Socket::process_incoming_dgram - ")
          ACE_TEXT ("received ")
          ACE_TEXT ("ICMP datagram with length of %b bytes (not counting ")
          ACE_TEXT ("IP-header): seq=%u, ttl=%d.\n"),
          icmplen, icmp->icmp_seq, ip->ip_ttl));

      return 0; //= success
    }

  ACELIB_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) ACE::Ping_Socket::process_incoming_dgram - ")
      ACE_TEXT ("received datagram that is not ICMP_ECHOREPLY.\n")));

  return -1;
}

int
ACE_Ping_Socket::send_echo_check (ACE_INET_Addr &remote_addr,
                                  bool to_connect)
{
  if (this->get_handle () == ACE_INVALID_HANDLE)
    {
      errno = EBADF;
      return -1;
    }

  sockaddr_in * const addr_connect {
    reinterpret_cast<sockaddr_in *> (remote_addr.get_addr ())};

  /*
   * Nulling port field to prevent strange behavior, when a raw
   * socket is "connected" to a sockaddr_in with a non-nulled port.
   */
  std::memset (&addr_connect->sin_port,
               0,
               sizeof (addr_connect->sin_port));

  // to connect the socket
  if (to_connect && !this->connected_socket_)
    {
      if (ACE_OS::connect (this->get_handle (),
                           reinterpret_cast<sockaddr *> (addr_connect),
                           remote_addr.get_size ()) == -1)
        {
          if (errno != EINTR)
            return -1;
       }
      this->connected_socket_ = true;
    }

  std::memset (this->icmp_send_buff_, 0, sizeof this->icmp_send_buff_);
  int const datalen {ICMP_DATA_LENGTH};
  struct icmp * const icmp_header {
    reinterpret_cast<struct icmp *> (this->icmp_send_buff_)};
  icmp_header->icmp_type = ICMP_ECHO;
  icmp_header->icmp_code = 0;
  icmp_header->icmp_id = ACE_OS::getpid () & 0xFFFF;
  icmp_header->icmp_seq = sequence_number_++;

#if defined (ACE_WIN32)
  icmp_header->icmp_data = GetTickCount ();
#else  /* #if defined (ACE_WIN32) */
  gettimeofday (reinterpret_cast<struct timeval *> (&icmp_header->icmp_data), nullptr);
#endif  /* #if defined (ACE_WIN32) */

  int const length_icmp {ICMP_MIN + datalen}; // checksum ICMP header and data.
  icmp_header->icmp_cksum = 0;
  icmp_header->icmp_cksum = inherited::calculate_checksum (
    reinterpret_cast<u_short *> (icmp_header), length_icmp);

  ssize_t const rval_send = send (icmp_send_buff_, length_icmp, remote_addr);
  if (rval_send != length_icmp)
    return -1;

  return 0;
}

int
ACE_Ping_Socket::make_echo_check (ACE_INET_Addr & remote_addr,
                                  bool to_connect,
                                  ACE_Time_Value const * timeout)
{
  int const rval_send = this->send_echo_check (remote_addr, to_connect);
  if (rval_send == -1)
    return -1;

  ACELIB_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) ACE_Ping_Socket::make_echo_check - sent %d.\n"),
      rval_send));

  return this->receive_echo_reply (timeout);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif  /* ACE_HAS_ICMP_SUPPORT == 1 */
