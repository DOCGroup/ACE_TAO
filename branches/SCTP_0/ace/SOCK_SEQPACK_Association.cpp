// SOCK_SEQPACK_Association.cpp
// $Id$

#include "ace/SOCK_SEQPACK_Association.h"
#include "ace/Auto_Ptr.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_SEQPACK_Association.i"
#endif

ACE_RCSID(ace, SOCK_SEQPACK_Association, "SOCK_SEQPACK_Association.cpp,v 4.8 2000/04/19 02:49:39 brunsch Exp")

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
ACE_SOCK_SEQPACK_Association::get_local_addrs (ACE_Addr *addrs, size_t size) const
{
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::get_local_addrs");

  // Array of sockaddr
  ACE_Auto_Array_Ptr<sockaddr> addr_structs;

  // Physical size of this array
  int physical_size = size * sizeof(sockaddr);

  /* Create the array */
  {
    sockaddr *addr_structs_bootstrap = 0;
    ACE_NEW_RETURN(addr_structs_bootstrap,
                   sockaddr[size],
                   -1);
    addr_structs.reset(addr_structs_bootstrap);
  }

  /* Clear the array */
  ACE_OS::memset(addr_structs.get(),
                 0,
                 physical_size);

  /* Populate the array with real values from the getsockname system call */
  if (ACE_OS::getsockname (this->get_handle (),
                           addr_structs.get(),
                           &physical_size) == -1)
    return -1;

  /* Copy each sockaddr to the address structure of an ACE_Addr from
     the passed-in array */
  for (size_t i = 0; i < size; ++i) {
    ACE_OS::memcpy(addrs[i].get_addr(),
                   &(addr_structs[i]),
                   addrs[i].get_size());

    addrs[i].set_type(addr_structs[i].sa_family);
  }

  return 0;
}

int
ACE_SOCK_SEQPACK_Association::get_remote_addrs (ACE_Addr *addrs, size_t size) const
{
  ACE_TRACE ("ACE_SOCK_SEQPACK_Association::get_remote_addrs");

  // Array of sockaddr
  ACE_Auto_Array_Ptr<sockaddr> addr_structs;

  // Physical size of this array
  int physical_size = size * sizeof(sockaddr);

  /* Create the array */
  {
    sockaddr *addr_structs_bootstrap = 0;
    ACE_NEW_RETURN(addr_structs_bootstrap,
                   sockaddr[size],
                   -1);
    addr_structs.reset(addr_structs_bootstrap);
  }

  /* Clear the array */
  ACE_OS::memset(addr_structs.get(),
                 0,
                 physical_size);

  /* Populate the array with real values from the getpeername system call */
  if (ACE_OS::getpeername (this->get_handle (),
                           addr_structs.get(),
                           &physical_size) == -1)
    return -1;

  /* Copy each sockaddr to the address structure of an ACE_Addr from
     the passed-in array */
  for (size_t i = 0; i < size; ++i) {
    ACE_OS::memcpy(addrs[i].get_addr(),
                   &(addr_structs[i]),
                   addrs[i].get_size());

    addrs[i].set_type(addr_structs[i].sa_family);
  }

  return 0;
}
