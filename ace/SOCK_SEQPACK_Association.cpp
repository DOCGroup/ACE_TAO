// SOCK_SEQPACK_Association.cpp

#include /**/ "ace/SOCK_SEQPACK_Association.h"
#include /**/ "ace/Auto_Ptr.h"
#include /**/ "ace/Log_Msg.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include /**/ "ace/SOCK_SEQPACK_Association.i"
#endif

ACE_RCSID(ace, SOCK_SEQPACK_Association, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_SEQPACK_Association)

void
ACE_SOCK_SEQPACK_Association::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::dump");
#endif /* ACE_HAS_DUMP */
}

int
ACE_SOCK_SEQPACK_Association::close (void)
{
#if defined (ACE_WIN32)
  // We need the following call to make things work correctly on
  // Win32, which requires use to do a <close_writer> before doing the
  // close in order to avoid losing data.  Note that we don't need to
  // do this on UNIX since it doesn't have this "feature".  Moreover,
  // this will cause subtle problems on UNIX due to the way that
  // fork() works.
  this->close_writer ();
#endif /* ACE_WIN32 */
  // Close down the socket.
  return ACE_SOCK::close ();
}

#if defined (ACE_HAS_LKSCTP)
int
ACE_SOCK_SEQPACK_Association::get_local_addrs (ACE_INET_Addr *addrs, size_t &size) const
{
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::get_local_addrs");
  /* 
    The size of ACE_INET_Addr must be large enough to hold the number of
    local addresses on the machine.  If the array is too small, the function
    will only return the number of addresses that will fit.  If the array is 
    too large, the 'size' parameter will be modified to indicate the number
    of addrs.  

    We will call sctp_getladdrs() which accepts 3 parameters
    1. a socket fd
    2. a sctp association_id which will be ignored since we are using 
       tcp sockets    
    3. a pointer to a sockaddr_storage

    lksctp/draft will allocate memory and we are responsible for freeing
    it by calling sctp_freeladdrs().  
  */

  sockaddr_in *si = 0;
  sockaddr_storage *laddrs = 0;
  int err = 0;
  size_t len = 0;
  
  err = sctp_getladdrs(this->get_handle(), 0, &laddrs);
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
      // first we cast the sockaddr_storage to sockaddr_in
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
  return 0;
}

#else

int
ACE_SOCK_SEQPACK_Association::get_local_addrs (ACE_INET_Addr *addrs, size_t &size) const
{
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::get_local_addrs");

  /*
    We will be calling ACE_OS::getsockname, which accepts (and
    potentially modifies) two reference parameters:

    1.  a sockaddr_in* that points to a buffer
    2.  an int* that points to the size of this buffer

    The OpenSS7 implementation of SCTP copies an array of ipv4
    sockaddr_in into the buffer.  Then, if the size of the buffer is
    greater than the size used, the size parameter is reduced
    accordingly.

  */

  // The array of sockaddr_in will be stored in an ACE_Auto_Array_Ptr,
  // which causes dynamically-allocated memory to be released as soon
  // as the ACE_Auto_Array_Ptr goes out of scope.
  ACE_Auto_Array_Ptr<sockaddr_in> addr_structs;

  // Allocate memory for this array.  Return -1 if the memory cannot
  // be allocated.  (This activity requires a temporary variable---a
  // bare sockaddr_in* --- because ACE_NEW_RETURN cannot act directory on
  // an ACE_Auto_Array_Ptr.)
  {
    sockaddr_in *addr_structs_bootstrap = 0;
    ACE_NEW_RETURN(addr_structs_bootstrap,
                   sockaddr_in[size],
                   -1);
    addr_structs.reset(addr_structs_bootstrap);
  }

  // Physical size of this array is its logical size multiplied by
  // the physical size of one of its elements.
  int physical_size = size * sizeof(sockaddr_in);

  /* Clear the array */
  ACE_OS::memset(addr_structs.get(),
                 0,
                 physical_size);

  /* Populate the array with real values from the getsockname system
     call.  The variables addr_structs and phycisal_size are
     modified. */
  if (ACE_OS::getsockname (this->get_handle (),
                           ACE_reinterpret_cast (sockaddr *,
                                                 addr_structs.get()),
                           &physical_size) == -1)
    return -1;

  /* Calculate the NEW physical size of the array */
  size = physical_size / sizeof (sockaddr_in);

  /* Copy each sockaddr_in to the address structure of an ACE_Addr from
     the passed-in array */
  for (size_t i = 0; i < size; ++i) {

    addrs[i].set_addr(&(addr_structs[i]), sizeof(sockaddr_in));
    addrs[i].set_type(addr_structs[i].sin_family);
    addrs[i].set_size(sizeof(sockaddr_in));
  }

  return 0;
}
#endif


#if defined (ACE_HAS_LKSCTP)
int 
ACE_SOCK_SEQPACK_Association::get_remote_addrs (ACE_INET_Addr *addrs, size_t &size) const
{
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::get_remote_addrs");
  /*
    The size of ACE_INET_Addr must be large enough to hold the number of
    remotes addresses in the association.  If the array is too small, the 
    function will only return the number of addresses that will fit.  If the 
    array is too large, the 'size' parameter will be modified to indicate
    the number of addrs.

    We will call sctp_getpaddrs() which accepts 3 parameters
    1. a socket fd
    2. a sctp association_id which will be ignored since we are using
       tcp sockets
    3. a pointer to a sockaddr_storage

    lksctp/draft will allocate memory and we are responsible for freeing
    it by calling sctp_freepaddrs().
  */

  sockaddr_in *si = 0;
  sockaddr_storage *paddrs = 0;
  int err = 0;
  size_t len = 0;

  err = sctp_getpaddrs(this->get_handle(), 0, &paddrs);
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
      // first we cast the sockaddr_storage to sockaddr_in
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
  return 0;
}

#else

int
ACE_SOCK_SEQPACK_Association::get_remote_addrs (ACE_INET_Addr *addrs, size_t &size) const
{
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::get_remote_addrs");

  /*
    We will be calling ACE_OS::getpeername, which accepts (and
    potentially modifies) two reference parameters:

    1.  a sockaddr_in* that points to a buffer
    2.  an int* that points to the size of this buffer

    The OpenSS7 implementation of SCTP copies an array of ipv4
    sockaddr_in into the buffer.  Then, if the size of the buffer is
    greater than the size used, the size parameter is reduced
    accordingly.

  */

  // The array of sockaddr_in will be stored in an ACE_Auto_Array_Ptr,
  // which causes dynamically-allocated memory to be released as soon
  // as the ACE_Auto_Array_Ptr goes out of scope.
  ACE_Auto_Array_Ptr<sockaddr_in> addr_structs;

  // Allocate memory for this array.  Return -1 if the memory cannot
  // be allocated.  (This activity requires a temporary variable---a
  // bare sockaddr_in* --- because ACE_NEW_RETURN cannot act directory on
  // an ACE_Auto_Array_Ptr.)
  {
    sockaddr_in *addr_structs_bootstrap = 0;
    ACE_NEW_RETURN(addr_structs_bootstrap,
                   sockaddr_in[size],
                   -1);
    addr_structs.reset(addr_structs_bootstrap);
  }

  // Physical size of this array is its logical size multiplied by
  // the physical size of one of its elements.
  int physical_size = size * sizeof(sockaddr_in);

  /* Clear the array */
  ACE_OS::memset(addr_structs.get(),
                 0,
                 physical_size);

  /* Populate the array with real values from the getpeername system
     call.  The variables addr_structs and phycisal_size are
     modified. */
  if (ACE_OS::getpeername (this->get_handle (),
                           ACE_reinterpret_cast (sockaddr *,
                                                 addr_structs.get()),
                           &physical_size) == -1)
    return -1;

  /* Calculate the NEW physical size of the array */
  size = physical_size / sizeof (sockaddr_in);

  /* Copy each sockaddr_in to the address structure of an ACE_Addr from
     the passed-in array */
  for (size_t i = 0; i < size; ++i) {

    addrs[i].set_addr(&(addr_structs[i]), sizeof(sockaddr_in));
    addrs[i].set_type(addr_structs[i].sin_family);
    addrs[i].set_size(sizeof(sockaddr_in));
  }

  return 0;
}
#endif

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Array_Ptr<sockaddr_in>;
template class ACE_Auto_Basic_Array_Ptr<sockaddr_in>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Array_Ptr<sockaddr_in>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<sockaddr_in>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
