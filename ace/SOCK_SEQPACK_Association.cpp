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
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::dump");
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Array_Ptr<sockaddr_in>;
template class ACE_Auto_Basic_Array_Ptr<sockaddr_in>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Array_Ptr<sockaddr_in>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<sockaddr_in>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
