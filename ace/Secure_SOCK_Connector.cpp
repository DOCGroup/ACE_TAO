// Secure_SOCK_Connector.cpp
// $Id$

#define ACE_HAS_OPENSSL
#if defined (ACE_HAS_OPENSSL)

#define ACE_BUILD_DLL

#include "Secure_SOCK_Connector.h"
#include "ace/Handle_Set.h"
#include "ace/INET_Addr.h"
#include <openssl/err.h>

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Secure_SOCK_Connector.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector (void)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector");
}

ACE_Secure_SOCK_Connector::~ACE_Secure_SOCK_Connector (void)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::~ACE_Secure_SOCK_Connector");
}

int
ACE_Secure_SOCK_Connector::reset_new_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::reset_new_handle");
  connector_.reset_new_handle (handle);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Secure_SOCK_Connector)

void
ACE_Secure_SOCK_Connector::dump (void) const
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::dump");
}
/*
int
ACE_Secure_SOCK_Connector::shared_connect_start (
                                          ACE_Secure_SOCK_Stream &new_stream,
					  ACE_Time_Value *timeout,
					  const ACE_Addr &local_sap,
					  int reuse_addr,
					  int protocol_family,
					  int protocol)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::shared_connect_start");
  return connector_.shared_connect_start (new_stream.peer (),
                                          timeout,
                                          local_sap,
                                          reuse_addr,
                                          protocol_family,
                                          protocol);
}

int
ACE_Secure_SOCK_Connector::shared_connect_finish (
                                          ACE_Secure_SOCK_Stream &new_stream,
					  ACE_Time_Value *timeout,
					  int result)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::shared_connect_finish");
  return connector_.shared_connect_finish (new_stream.peer (),
                                           timeout,
                                           result);
  // Never add anything else in here, it will be bypassed.
}
// Actively connect and produce a new ACE_Secure_SOCK_Stream if things go well...
*/
int
ACE_Secure_SOCK_Connector::connect (ACE_Secure_SOCK_Stream &new_stream,
			     const ACE_Addr &remote_sap,
			     ACE_Time_Value *timeout,
			     const ACE_Addr &local_sap,
			     int reuse_addr,
			     int flags,
			     int perms,
			     int protocol_family,
			     int protocol)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::connect");
  if (connector_.connect (new_stream.peer (),
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
  return shared_ssl_init (new_stream);

}

int
ACE_Secure_SOCK_Connector::connect (ACE_Secure_SOCK_Stream &new_stream,
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
  ACE_TRACE ("ACE_Secure_SOCK_Connector::connect");
  if (connector_.connect (new_stream.peer (),
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
  return shared_ssl_init (new_stream);
}

// Try to complete a non-blocking connection.

int
ACE_Secure_SOCK_Connector::complete (ACE_Secure_SOCK_Stream &new_stream,
			      ACE_Addr *remote_sap,
			      ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::complete");
  if (connector_.complete (new_stream.peer (),
                           remote_sap,
                           tv) == -1) {
    return -1;
  }
  else {
    //jwh1: Check to see if ssl stuff has completed.
  }
  return 0;
}

ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector (
                                        ACE_Secure_SOCK_Stream &new_stream,
					const ACE_Addr &remote_sap,
					ACE_Time_Value *timeout,
					const ACE_Addr &local_sap,
					int reuse_addr,
					int flags,
					int perms,
					int protocol_family,
					int protocol)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector");

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
                  "ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector"
               )));

  shared_ssl_init (new_stream);
  //jwh1: Wrap this in error catching code
}



ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector (
                                        ACE_Secure_SOCK_Stream &new_stream,
					const ACE_Addr &remote_sap,
					ACE_QoS_Params qos_params,
					ACE_Time_Value *timeout,
					const ACE_Addr &local_sap,
                                        ACE_Protocol_Info *protocolinfo = 0,
                                        ACE_SOCK_GROUP g = 0,
                                        u_long flags,
					int reuse_addr,
					int perms,
					int protocol_family,
					int protocol)
{
  ACE_TRACE ("ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector");

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
                  "ACE_Secure_SOCK_Connector::ACE_Secure_SOCK_Connector"
              )));

  shared_ssl_init (new_stream);
  //jwh1: Wrap this in error catching code.
}
#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2);}

int
ACE_Secure_SOCK_Connector::shared_ssl_init (ACE_Secure_SOCK_Stream& new_stream)
{

  SSL_library_init();
  SSL_METHOD* meth = SSLv2_client_method();
  SSL_load_error_strings();
  SSL_CTX* ctx = SSL_CTX_new (meth);                        CHK_NULL(ctx);

  SSL* ssl = SSL_new (ctx);                         CHK_NULL(ssl);
  SSL_set_fd (ssl, new_stream.get_handle ());

  int err = SSL_connect (ssl);                     CHK_SSL(err);

  new_stream.session (ssl);

  return err;
}

#endif /* defined (ACE_HAS_OPENSSL) */
