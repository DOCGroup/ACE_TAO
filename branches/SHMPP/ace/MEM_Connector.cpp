// MEM_Connector.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/MEM_Connector.h"

ACE_RCSID(ace, MEM_Connector, "$Id$")

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_Connector.i"
#endif

ACE_ALLOC_HOOK_DEFINE(ACE_MEM_Connector)

void
ACE_MEM_Connector::dump (void) const
{
  ACE_TRACE ("ACE_MEM_Connector::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_MEM_Connector::ACE_MEM_Connector (void)
{
  ACE_TRACE ("ACE_MEM_Connector::ACE_MEM_Connector");
}

// Establish a connection.
ACE_MEM_Connector::ACE_MEM_Connector (ACE_MEM_Stream &new_stream,
                                      const ACE_INET_Addr &remote_sap,
                                      ACE_Time_Value *timeout,
                                      const ACE_Addr &local_sap,
                                      int reuse_addr,
                                      int flags,
                                      int perms,
                                      int protocol_family,
                                      int protocol)
{
  ACE_TRACE ("ACE_MEM_Connector::ACE_MEM_Connector");
  // This is necessary due to the weird inheritance relationships of
  // ACE_MEM_Stream.
  this->connect (new_stream,
                 remote_sap,
                 timeout,
                 local_sap,
                 reuse_addr,
                 flags,
                 perms,
                 protocol_family,
                 protocol);
}

int
ACE_MEM_Connector::connect (ACE_MEM_Stream &new_stream,
                            const ACE_INET_Addr &remote_sap,
                            ACE_Time_Value *timeout,
                            const ACE_Addr &local_sap,
                            int reuse_addr,
                            int flags,
                            int perms,
                            int protocol_family,
                            int protocol)
{
  ACE_TRACE ("ACE_MEM_Connector::connect");

  ACE_SOCK_Stream temp_stream;
  if (ACE_SOCK_Connector::connect (temp_stream, remote_sap,
                                   timeout, local_sap,
                                   reuse_addr, flags, perms,
                                   protocol_family, protocol) == -1)
    return -1;

  ACE_HANDLE new_handle = temp_stream.get_handle ();
  new_stream.set_handle (new_handle);
  // Do not close the handle.

  // now we should setup the mmap malloc.
  char buf[MAXPATHLEN];

  // @@ Need to handle timeout here.
  ACE_INT16 buf_len;
  // Byte-order is not a problem for this read.
  if (ACE::recv (new_handle, &buf_len, sizeof (buf_len)) == -1)
    return -1;

  if (ACE::recv (new_handle, buf, buf_len) == -1)
    return -1;

  if (new_stream.create_shm_malloc (buf) == -1)
    return -1;

  return 0;
}
