// $Id$

#include "ace/Ping_Socket.h"

#if defined (ACE_HAS_ICMP_SUPPORT) && (ACE_HAS_ICMP_SUPPORT == 1)

#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
# include "ace/Ping_Socket.inl"
#endif  /* !__ACE_INLINE__ */


ACE_RCSID (ace,
           Ping_Socket,
           "$Id$")


namespace ACE
{
  ACE_ALLOC_HOOK_DEFINE (Ping_Socket)
}

//---------------------------------------------------------------------------
// Better to arrange some os_include/netinet/ip.h and
// os_include/netinet/icmp.h files ?
//---------------------------------------------------------------------------

#if !defined (ACE_WIN32)

/*
 * This is where ICMP-related stuff is defined on any sane system...
 */
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#else  /* #if ! defined (ACE_WIN32) */

/*
 * This was a surpise to me...  This stuff is not defined anywhere under MSVC.
 * These values have only been checked for NT4 and Win2K.  They were taken from
 * the MSDN ping.c program and modified.
 */

#define ICMP_ECHO       8
#define ICMP_ECHOREPLY  0

struct ip
{
    unsigned int    ip_hl:4;            // length of the header
    unsigned int    version:4;          // Version of IP
    unsigned char   tos;                // Type of service
    unsigned short  total_len;          // total length of the packet
    unsigned short  ident;              // unique identifier
    unsigned short  frag_and_flags;     // flags
    unsigned char   ip_ttl;             // Time to live
    unsigned char   proto;              // protocol (TCP, UDP etc)
    unsigned short  checksum;           // IP checksum
    unsigned int    sourceIP;
    unsigned int    destIP;
};

struct icmp
{
    unsigned char  icmp_type;
    unsigned char  icmp_code;      // type sub code
    unsigned short icmp_cksum;
    unsigned short icmp_id;
    unsigned short icmp_seq;
    unsigned long  icmp_data;      // time data
};

#endif /* #if ! defined (ACE_WIN32) */




int const ICMP_MIN = 8;  // Minimal size of ICMP packet, header only
int const ICMP_DATA_LENGTH = 56;  // For ICMP data with Echo request
ACE_Time_Value const ACE::Ping_Socket::time_default_ (0, 500000);


void
ACE::Ping_Socket::dump (void) const
{
    ACE_TRACE ("ACE::Ping_Socket::dump");
}

ACE::Ping_Socket::Ping_Socket (void)
{
    ACE_TRACE ("ACE::Ping_Socket::Ping_Socket");
}

ACE::Ping_Socket::Ping_Socket (ACE_Addr const & local,
                               int protocol,
                               int reuse_addr)
  : sequence_number_ (0),
    connected_socket_ (0)
{
  ACE_TRACE ("ACE::Ping_Socket::Ping_Socket");

  ACE_OS::memset (icmp_send_buff_, 0, sizeof (icmp_send_buff_));
  ACE_OS::memset (icmp_recv_buff_, 0, sizeof (icmp_recv_buff_));

  if (this->open (local, protocol, reuse_addr) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "ACE::Ping_Socket::Ping_Socket - open() error.\n"));
    }

  // trying to increase the size of socket receive buffer - some
  // protection from multiple responces e.g., when falling to the
  // multi-cast address
  int size = 64 * 1024;
  ACE_SOCK::set_option (SOL_SOCKET,
                        SO_RCVBUF,
                        (void *) &size,
                        sizeof (size));
}

ACE::Ping_Socket::~Ping_Socket (void)
{
  ACE_TRACE ("ACE::Ping_Socket::~Ping_Socket");
}

int
ACE::Ping_Socket::open (ACE_Addr const & local,
                        int protocol,
                        int reuse_addr)
{
  ACE_TRACE ("ACE::Ping_Socket::open");
  return inherited::open (local, protocol, reuse_addr);
}

int
ACE::Ping_Socket::receive_echo_reply (ACE_Time_Value const * timeout)
{
  ACE_TRACE ("ACE::Ping_Socket::receive_echo_reply");

  ACE_Time_Value before, after;

  ACE_OS::memset (icmp_recv_buff_, 0, sizeof icmp_recv_buff_);
  before = ACE_OS::gettimeofday ();
  ACE_Time_Value* delta = (ACE_Time_Value*) timeout;

  while (1)
    {
      // = select() does not work with raw sockets properly on some
      //   platforms, if connect ()
      // = on such socket performed on sockaddr_in with non-nulled
      //   port field
      //
      int rval_recv = inherited::recv (icmp_recv_buff_,
                                       sizeof icmp_recv_buff_,
                                       0,
                                       (ACE_Time_Value const *) delta);
      if (rval_recv < 0)
        {
          if (errno == EINTR)
            {
              after = ACE_OS::gettimeofday ();
              ACE_Time_Value time_left = *timeout - after + before;

              // if more than 1 ms left, lets come to wait on select()
              if (time_left.msec () > 1)
                {
                  delta = &time_left; // coming back to wait on select()
                  continue;
                }
            }
          return -1;
        }
      else if (!process_incoming_dgram (icmp_recv_buff_, rval_recv))
        {
          return 0; //= success
        }
      else
        {
          after = ACE_OS::gettimeofday ();

          if ((after - before) >= *timeout)
            {
              errno = ETIMEDOUT;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%p\n", "(%P|%t) "
                                 "ACE::Ping_Socket::receive_echo_reply - "
                                 "select returned 0, timed out."),
                                -1);
            }
          // new timeout, we are coming back to sit on select
          ACE_Time_Value new_timeout = *timeout - after + before;
          delta = &new_timeout;
        }
      // to null the buffer prior to putting it to job
      ACE_OS::memset (icmp_recv_buff_, 0, sizeof icmp_recv_buff_);
    }
  return 0;
}

int
ACE::Ping_Socket::process_incoming_dgram (char * ptr, ssize_t len)
{
  int hlen1, icmplen;
  struct ip * ip;
  struct icmp * icmp;

  ip = (struct ip *) ptr;                 // start of IP header
  hlen1 = ip->ip_hl << 2;                 // length of IP header
  icmp = (struct icmp *) (ptr + hlen1);   // start of ICMP header

  if ((icmplen = len - hlen1) < ICMP_MIN)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) ACE::Ping_Socket::process_incoming_dgram - "
                  "ICMP lenght is %d < 8.\n",
                  icmplen));
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) ACE::Ping_Socket::process_incoming_dgram - "
                         "The ICMP header either not received or is "
                         "corrupted."),
                          -1);
    }

  if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) ACE::Ping_Socket::process_incoming_dgram - "
                  "ICMP_ECHOREPLY received.\n"));

      if (icmp->icmp_id != getpid ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) ACE::Ping_Socket::"
                             "process_incoming_dgram "
                             "- The ICMP header received is a reply "
                             "to request of another process."),
                            -1);
        }
      if (icmplen < 16)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) ACE::Ping_Socket::"
                             "process_incoming_dgram - ICMP lenght is "
                             "%d < 16.",
                               icmplen),
                            -1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) ACE::Ping_Socket::process_incoming_dgram - "
                  "received "
                  "ICMP datagram with length of %d bytes (not counting "
                  "IP-header): seq=%u, ttl=%d.\n\n",
                  icmplen, icmp->icmp_seq, ip->ip_ttl)) ;

      return 0; //= success
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) ACE::Ping_Socket::process_incoming_dgram - "
              "received datagram that is not ICMP_ECHOREPLY.\n"));

  return -1;
}

int
ACE::Ping_Socket::send_echo_check (ACE_INET_Addr &remote_addr,
                                   int to_connect)
{
  if (this->get_handle () == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) ACE::Ping_Socket::make_echo_check - "
                         "invalid descriptor."),
                        -1);
    }

  sockaddr_in *addr_connect = NULL;
  addr_connect = (sockaddr_in *) remote_addr.get_addr ();

  /*
   * Nulling port field to prevent strange behavior, when a raw
   * socket is "connected" to a sockaddr_in with a non-nulled port.
   */
  ACE_OS::memset ((void*) &addr_connect->sin_port,
                  0,
                  sizeof (addr_connect->sin_port));

  // to connect the socket
  if (to_connect && !this->connected_socket_)
    {
      if (ACE_OS::connect (this->get_handle (),
                           (sockaddr*) addr_connect,
                           remote_addr.get_size ()) == -1)
        {
          if (errno != EINTR)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%p\n", "(%P|%t) "
                                 "ACE::Ping_Socket::make_echo_check - "
                                 "connect() failed."),
                                -1);
            }
        }
      this->connected_socket_ = 1;
    }

  ACE_OS::memset (this->icmp_send_buff_, 0, sizeof this->icmp_send_buff_);
  int datalen = ICMP_DATA_LENGTH;
  struct icmp *_icmp;

  _icmp = (struct icmp *) this->icmp_send_buff_;
  _icmp->icmp_type = ICMP_ECHO;
  _icmp->icmp_code = 0;
  _icmp->icmp_id = getpid ();
  _icmp->icmp_seq = sequence_number_++;

#if defined (ACE_WIN32)
  _icmp->icmp_data = GetTickCount ();
#else  /* #if defined (ACE_WIN32) */
  gettimeofday ((struct timeval *) &_icmp->icmp_data, NULL);
#endif  /* #if defined (ACE_WIN32) */

  int length_icmp = ICMP_MIN + datalen; // checksum ICMP header and data.
  _icmp->icmp_cksum = 0;
  _icmp->icmp_cksum = inherited::calculate_checksum ((u_short *) _icmp,
                                                     length_icmp);
  int rval_send = -1;

  if ((rval_send = send ((void const *) icmp_send_buff_,
                         length_icmp,
                         remote_addr)) != length_icmp)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) ACE::Ping_Socket::send_echo_check - "
                         "send() failed, sent %d bytes instead of %d.\n",
                         rval_send, length_icmp),
                        -1);
    }
  return 0;
}

int
ACE::Ping_Socket::make_echo_check (ACE_INET_Addr & remote_addr,
                                   int to_connect,
                                   ACE_Time_Value const * timeout)
{
  int rval_send = -1;

  if ((rval_send = this->send_echo_check (remote_addr,
                                          to_connect)) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) ACE::Ping_Socket::make_echo_check - "
                         "send_echo_check failed.\n"),
                        -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) ACE::Ping_Socket::make_echo_check - sent %d.\n",
              rval_send));

  return receive_echo_reply (timeout);
}

#endif  /* ACE_HAS_ICMP_SUPPORT == 1 */
