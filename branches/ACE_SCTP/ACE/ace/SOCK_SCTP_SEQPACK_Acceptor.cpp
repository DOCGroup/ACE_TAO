// $Id$

#include "ace/SOCK_SCTP_SEQPACK_Acceptor.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/os_fcntl.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_SCTP_SEQPACK_Acceptor)

ACE_SOCK_SCTP_SEQPACK_Acceptor::ACE_SOCK_SCTP_SEQPACK_Acceptor (void)
{
  ACE_TRACE ("ACE_SOCK_SCTP_SEQPACK_Acceptor::ACE_SOCK_SCTP_SEQPACK_Acceptor");
}

ACE_SOCK_SCTP_SEQPACK_Acceptor::~ACE_SOCK_SCTP_SEQPACK_Acceptor (void)
{
  ACE_TRACE ("ACE_SOCK_SCTP_SEQPACK_Acceptor::~ACE_SOCK_SCTP_SEQPACK_Acceptor");
}

int
ACE_SOCK_SCTP_SEQPACK_Acceptor::open (const ACE_Multihomed_INET_Addr &local_sap,
                                      int reuse_addr,
                                      int protocol_family,
                                      int backlog)
{
  int error = 0;
 
  // Call the base class open()
  if(ACE_SOCK_SCTP_SEQPACK::open(local_sap,
                                 reuse_addr,
                                 protocol_family) == -1)
  {
    error = 1;
  }
  

  // If the base class open succeeded call listen to enable passive mode 
  // socket
  if(ACE_OS::listen (this->get_handle (),
                     backlog) == -1)
  {
    error = 1;
  }

  // Return -1 on error
  return error ? -1 : 0;
}


ACE_SOCK_SCTP_SEQPACK_Acceptor::ACE_SOCK_SCTP_SEQPACK_Acceptor (const ACE_Multihomed_INET_Addr &local_sap,
                                                                int reuse_addr,
                                                                int protocol_family,
                                                                int backlog)
{
  ACE_TRACE ("ACE_SOCK_SCTP_SEQPACK_Acceptor::ACE_SOCK_SCTP_SEQPACK_Acceptor");

  if (this->open (local_sap,
                  reuse_addr,
                  protocol_family,
                  backlog) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_SOCK_SCTP_SEQPACK_Acceptor")));
}

int
ACE_SOCK_SCTP_SEQPACK_Acceptor::close (void)
{
  return ACE_SOCK::close ();
}

ACE_END_VERSIONED_NAMESPACE_DECL
