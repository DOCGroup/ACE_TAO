// -*- C++ -*-
// $Id$


#include "SSL_SOCK_Connector.h"

#if defined (ACE_HAS_SSL)

#include "ace/Handle_Set.h"
#include "ace/INET_Addr.h"

#include <openssl/err.h>

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Connector.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */


ACE_ALLOC_HOOK_DEFINE(ACE_SSL_SOCK_Connector)

int
ACE_SSL_SOCK_Connector::shared_connect_start (ACE_SSL_SOCK_Stream &new_stream,
                                              ACE_Time_Value *timeout,
                                              const ACE_Addr &local_sap)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::shared_connect_start");

  if (local_sap != ACE_Addr::sap_any)
  {
      sockaddr *laddr = ACE_reinterpret_cast (sockaddr *,
                                              local_sap.get_addr ());
      size_t size = local_sap.get_size ();

      if (ACE_OS::bind (new_stream.get_handle (),
			laddr,
			size) == -1)
	{
	  // Save/restore errno.
	  ACE_Errno_Guard error (errno);
	  new_stream.close ();
	  return -1;
	}
    }

  // Enable non-blocking, if required.
  if (timeout != 0 && new_stream.disable (ACE_NONBLOCK) == -1)
    return -1;
  else
    return 0;
}

int
ACE_SSL_SOCK_Connector::shared_connect_finish (ACE_SSL_SOCK_Stream &new_stream,
                                               ACE_Time_Value *timeout,
                                               int result)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::shared_connect_finish");
  // Save/restore errno.
  ACE_Errno_Guard error (errno);

  if (result == -1 && timeout != 0)
    {
      // Check whether the connection is in progress.
      if (error == EINPROGRESS || error == EWOULDBLOCK)
	{
	  // This expression checks if we were polling.
	  if (timeout->sec () == 0
              && timeout->usec () == 0)
	    error = EWOULDBLOCK;
	  // Wait synchronously using timeout.
	  else if (this->complete (new_stream,
                                   0,
                                   timeout) == -1)
	    error = errno;
	  else
	    return 0;
	}
    }

  // EISCONN is treated specially since this routine may be used to
  // check if we are already connected.
  if (result != -1 || error == EISCONN)
    // Start out with non-blocking disabled on the <new_stream>.
    new_stream.disable (ACE_NONBLOCK);
  else if (!(error == EWOULDBLOCK || error == ETIMEDOUT))
    new_stream.close ();

  return result;

}

int
ACE_SSL_SOCK_Connector::ssl_connect (ACE_SSL_SOCK_Stream &new_stream)
{
  if (SSL_is_init_finished (new_stream.ssl ()))
    return 0;

  // @@ This is awkward.
  new_stream.set_handle (new_stream.peer ().get_handle ());

  int status = ::SSL_connect (new_stream.ssl ());
  if (status < 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "  ACE_SSL::connect - failed (%d)\n",
      //             status));
      if (::BIO_sock_should_retry (status))
        {
          errno = EAGAIN;
        }
      else
        ERR_print_errors_fp (stderr);

      return -1;
    }

  return 0;
}

int
ACE_SSL_SOCK_Connector::connect (ACE_SSL_SOCK_Stream &new_stream,
                                 const ACE_Addr &remote_sap,
                                 ACE_Time_Value *timeout,
                                 const ACE_Addr &local_sap,
                                 int reuse_addr,
                                 int flags,
                                 int perms,
                                 int protocol_family,
                                 int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::connect");
  if ((new_stream.get_handle () == ACE_INVALID_HANDLE) &&
      this->connector_.connect (new_stream.peer (),
                                remote_sap,
                                timeout,
                                local_sap,
                                reuse_addr,
                                flags,
                                perms,
                                protocol_family,
                                protocol) == -1) {
    return -1;
  }

  return this->ssl_connect (new_stream);
}

int
ACE_SSL_SOCK_Connector::connect (ACE_SSL_SOCK_Stream &new_stream,
                                 const ACE_Addr &remote_sap,
                                 ACE_QoS_Params qos_params,
                                 ACE_Time_Value *timeout,
                                 const ACE_Addr &local_sap,
                                 ACE_Protocol_Info *protocolinfo,
                                 ACE_SOCK_GROUP g,
                                 u_long flags,
                                 int reuse_addr,
                                 int perms,
                                 int protocol_family,
                                 int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::connect");
  if ((new_stream.get_handle () == ACE_INVALID_HANDLE) &&
      connector_.connect (new_stream.peer (),
                          remote_sap,
                          qos_params,
                          timeout,
                          local_sap,
                          protocolinfo,
                          g,
                          flags,
                          reuse_addr,
                          perms,
                          protocol_family,
                          protocol) == -1) {
    return -1;
  }

  return this->ssl_connect (new_stream);
}

// Try to complete a non-blocking connection.

int
ACE_SSL_SOCK_Connector::complete (ACE_SSL_SOCK_Stream &new_stream,
                                  ACE_Addr *remote_sap,
                                  ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::complete");
  if (this->connector_.complete (new_stream.peer (),
                                 remote_sap,
                                 tv) == -1) {
    return -1;
  }

  return this->ssl_connect (new_stream);
}


ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector (
                                        ACE_SSL_SOCK_Stream &new_stream,
					const ACE_Addr &remote_sap,
					ACE_Time_Value *timeout,
					const ACE_Addr &local_sap,
					int reuse_addr,
					int flags,
					int perms,
					int protocol_family,
					int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector");
  if (this->connect (new_stream,
		     remote_sap,
		     timeout,
		     local_sap,
		     reuse_addr,
		     flags,
		     perms,
		     protocol_family,
		     protocol) == -1
      && timeout != 0
      && !(errno == EWOULDBLOCK || errno == ETIME))
    ACE_ERROR ((LM_ERROR,
		ASYS_TEXT ("%p\n"),
		ASYS_TEXT (
                  "ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector"
               )));
}

ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector (
                                        ACE_SSL_SOCK_Stream &new_stream,
					const ACE_Addr &remote_sap,
					ACE_QoS_Params qos_params,
					ACE_Time_Value *timeout,
					const ACE_Addr &local_sap,
                                        ACE_Protocol_Info *protocolinfo,
                                        ACE_SOCK_GROUP g,
                                        u_long flags,
					int reuse_addr,
					int perms,
					int protocol_family,
					int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector");

  if (this->connect (new_stream,
		     remote_sap,
		     qos_params,
		     timeout,
		     local_sap,
                     protocolinfo,
                     g,
		     flags,
                     reuse_addr,
		     perms,
		     protocol_family,
		     protocol) == -1
      && timeout != 0
      && !(errno == EWOULDBLOCK || errno == ETIME))
    ACE_ERROR ((LM_ERROR,
		ASYS_TEXT ("%p\n"),
		ASYS_TEXT (
                  "ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector"
              )));
}

#endif /* ACE_HAS_SSL */
