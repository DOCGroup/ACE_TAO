// $Id$

#include "ace/SOCK_SCTP.h"

#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

ACE_RCSID(ace, SOCK_SCTP, "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_SCTP)


ACE_SOCK_SCTP::ACE_SOCK_SCTP()
{
}

ACE_SOCK_SCTP::ACE_SOCK_SCTP (ACE_HANDLE h)
{
  this->set_handle(h);
}

ACE_SOCK_SCTP::~ACE_SOCK_SCTP()
{
}

int
ACE_SOCK_SCTP::close (void)
{
  // Close down the socket.
  return ACE_SOCK::close ();
}

ssize_t ACE_SOCK_SCTP::recvmsg(void* msg,
                               size_t msgsz,
                               ACE_Multihomed_INET_Addr& from_addr,
                               sctp_sndrcvinfo* sinfo,
                               int* msg_flags
                               )
{
  ssize_t rdsz;
  sockaddr_in frm;
  bzero(&frm, sizeof(frm));
  socklen_t frm_len = 0;
  
  rdsz = sctp_recvmsg(this->get_handle(),
                      msg,
                      msgsz,
                      (sockaddr*)&frm,
                      &frm_len,
                      sinfo,
                      msg_flags);

  // *** TO-DO: Initialize from_addr
  
  return rdsz;
}

ssize_t ACE_SOCK_SCTP::sendmsg(const void* msg,
                                       size_t msgsz,
                                       ACE_Multihomed_INET_Addr& to_addr,
                                       uint32_t ppid,
                                       uint32_t flags,
                                       uint16_t stream,
                                       uint32_t timetolive,
                                       uint32_t context)
{
  ssize_t sndsize;

  sndsize = sctp_sendmsg(this->get_handle(),
                         msg,
                         msgsz,
                         (sockaddr*)to_addr.get_addr(),
                         to_addr.get_addr_size(),
                         ppid,
                         flags,
                         stream,
                         timetolive,
                         context);
  
  return sndsize; 
}

int
ACE_SOCK_SCTP::get_local_addrs (ACE_INET_Addr *addrs, size_t &size, int assoc_id) const
{
  ACE_TRACE ("ACE_SOCK_SCTP::get_local_addrs");

#if defined (ACE_HAS_LKSCTP)
  /*
    The size of ACE_INET_Addr must be large enough to hold the number of
    local addresses on the machine.  If the array is too small, the function
    will only return the number of addresses that will fit.  If the array is
    too large, the 'size' parameter will be modified to indicate the number
    of addrs.

    We will call sctp_getladdrs() which accepts 3 parameters
    1. a socket fd
    2. a sctp association_id (should be set to 0 for one-to-one style sockets) 
    3. a pointer to sockaddr

    lksctp/draft will allocate memory and we are responsible for freeing
    it by calling sctp_freeladdrs().
  */

  sockaddr_in *si = 0;
  sockaddr *laddrs = 0;
  int err = 0;
  size_t len = 0;

  err = sctp_getladdrs(this->get_handle(), assoc_id, &laddrs);
  if (err > 0)
  {
    len = err;
    // check to see if we have more addresses than we have
    // space in our ACE_INET_Addr array
    if (len > size)
    {
      // since our array is too small, we will only copy the first
      // few that fit
      len = size;
    }

    for (size_t i = 0; i < len; i++)
    {
      // first we cast the sockaddr to sockaddr_in
      // since we only support ipv4 at this time.
      si = (sockaddr_in *) (&(laddrs[i]));

      // now we fillup the ace_inet_addr array
      addrs[i].set_addr(si, sizeof(sockaddr_in));
      addrs[i].set_type(si->sin_family);
      addrs[i].set_size(sizeof(sockaddr_in));
    }
  }
  else /* err < 0 */
  {
    // sctp_getladdrs will return -1 on error
    return -1;
  }

  // indicate the num of addrs returned to the calling function
  size = len;

  // make sure we free the struct using the system function
  sctp_freeladdrs(laddrs);

#endif /* ACE_HAS_LKSCTP */
  return 0;
}


int
ACE_SOCK_SCTP::get_remote_addrs (ACE_INET_Addr *addrs, size_t &size, int assoc_id) const
{
  ACE_TRACE ("ACE_SOCK_SCTP::get_remote_addrs");
#if defined (ACE_HAS_LKSCTP)
  /*
    The size of ACE_INET_Addr must be large enough to hold the number of
    remotes addresses in the association.  If the array is too small, the
    function will only return the number of addresses that will fit.  If the
    array is too large, the 'size' parameter will be modified to indicate
    the number of addrs.

    We will call sctp_getpaddrs() which accepts 3 parameters
    1. a socket fd
    2. a sctp association_id (0 for 1-to-1 style sockets)
    3. a pointer to a sockaddr

    lksctp/draft will allocate memory and we are responsible for freeing
    it by calling sctp_freepaddrs().
  */

  sockaddr_in *si = 0;
  sockaddr *paddrs = 0;
  int err = 0;
  size_t len = 0;

  err = sctp_getpaddrs(this->get_handle(), assoc_id, &paddrs);
  if (err > 0)
  {
    len = err;
    // check to see if we have more addresses than we have
    // space in our ACE_INET_Addr array
    if (len > size)
    {
      // since our array is too small, we will only copy the first
      // few that fit
      len = size;
    }

    for (size_t i = 0; i < len; i++)
    {
      // first we cast the sockaddr to sockaddr_in
      // since we only support ipv4 at this time.
      si = (sockaddr_in *) (&(paddrs[i]));

      // now we fillup the ace_inet_addr array
      addrs[i].set_addr(si, sizeof(sockaddr_in));
      addrs[i].set_type(si->sin_family);
      addrs[i].set_size(sizeof(sockaddr_in));
    }
  }
  else /* err < 0 */
  {
    // sctp_getpaddrs will return -1 on error
    return -1;
  }

  // indicate the num of addrs returned to the calling function
  size = len;

  // make sure we free the struct using the system function
  sctp_freepaddrs(paddrs);

#endif /* ACE_HAS_LKSCTP */
  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
