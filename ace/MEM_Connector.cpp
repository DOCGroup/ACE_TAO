// MEM_Connector.cpp
// $Id$

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
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_MEM_Connector::ACE_MEM_Connector (void)
  : malloc_options_ (ACE_DEFAULT_BASE_ADDR, 0)
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
                                      int protocol)
  : malloc_options_ (ACE_DEFAULT_BASE_ADDR, 0)
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
                            int protocol)
{
  ACE_TRACE ("ACE_MEM_Connector::connect");

  if (!this->address_.same_host (remote_sap))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("(%P|%t) MEM_Connector can't connect ")
                       ACE_LIB_TEXT ("to %s:%d which is not a local endpoint"),
                       remote_sap.get_host_name (),
                       remote_sap.get_port_number ()),
                      -1);
  else
    this->address_.set_port_number (remote_sap.get_port_number ());


  ACE_SOCK_Stream temp_stream;

  if (ACE_SOCK_Connector::connect (temp_stream,
                                   this->address_.get_local_addr (),
                                   timeout, local_sap,
                                   reuse_addr, flags, perms,
                                   PF_INET, protocol) == -1)
    return -1;

  ACE_HANDLE new_handle = temp_stream.get_handle ();
  new_stream.set_handle (new_handle);
  // Do not close the handle.

  // now we should setup the mmap malloc.
  ACE_TCHAR buf[MAXPATHLEN];

  // @@ Need to handle timeout here.
  ACE_INT16 buf_len;
  // Byte-order is not a problem for this read.
  if (ACE::recv (new_handle, &buf_len, sizeof (buf_len)) == -1)
    return -1;

  if (ACE::recv (new_handle, buf, buf_len) == -1)
    return -1;

  if (new_stream.create_shm_malloc (buf, &this->malloc_options_) == -1)
    return -1;

  return 0;
}
