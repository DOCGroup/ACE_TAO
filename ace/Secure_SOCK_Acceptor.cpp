// Secure_SOCK_Acceptor.cpp
// $Id$
#define ACE_BUILD_DLL
#include "Secure_SOCK_Acceptor.h"

#define ACE_HAS_OPENSSL
#if defined (ACE_HAS_OPENSSL)

#include <ace/Synch.h>
#include <openssl/err.h>

ACE_ALLOC_HOOK_DEFINE(ACE_Secure_SOCK_Acceptor)

// Do nothing routine for constructor.

ACE_Secure_SOCK_Acceptor::ACE_Secure_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::ACE_Secure_SOCK_Acceptor");
}

ACE_Secure_SOCK_Acceptor::ACE_Secure_SOCK_Acceptor (const ACE_Addr &local_sap,
                                                    int reuse_addr,
                                                    int protocol_family,
                                                    int backlog,
                                                    int protocol)
  : acceptor_ (local_sap, reuse_addr, protocol_family, backlog, protocol)
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::ACE_Secure_SOCK_Acceptor");
}
// Performs the timed accept operation.

ACE_Secure_SOCK_Acceptor::ACE_Secure_SOCK_Acceptor (const ACE_Addr &local_sap,
                                                    ACE_Protocol_Info *protocolinfo,
                                                    ACE_SOCK_GROUP g,
                                                    u_long flags,
                                                    int reuse_addr,
                                                    int protocol_family,
                                                    int backlog = ACE_DEFAULT_BACKLOG,
                                                    int protocol = 0)
  : acceptor_ (local_sap, protocolinfo, g, flags, reuse_addr, protocol_family, backlog, protocol)
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::ACE_Secure_SOCK_Acceptor");
}

int
ACE_Secure_SOCK_Acceptor::open (const ACE_Addr &local_sap,
                                int reuse_addr,
                                int protocol_family,
                                int backlog,
                                int protocol)
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::open");
  return acceptor_.open (local_sap,
                         reuse_addr,
                         protocol_family,
                         backlog,
                         protocol);
}

ACE_Secure_SOCK_Acceptor::~ACE_Secure_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::~ACE_Secure_SOCK_Acceptor");
}

// General purpose routine for accepting new connections.
int
ACE_Secure_SOCK_Acceptor::accept (ACE_Secure_SOCK_Stream &new_stream,
                                  ACE_Addr *remote_addr,
                                  ACE_Time_Value *timeout,
                                  int restart,
                                  int reset_new_handle) const
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::accept");
  shared_ssl_init_one (new_stream);
  if (acceptor_.accept (new_stream.peer (),
                        remote_addr,
                        timeout,
                        restart,
                        reset_new_handle) == -1) {
    return -1;
  }
  return shared_ssl_init_two (new_stream);
}

int
ACE_Secure_SOCK_Acceptor::accept (ACE_Secure_SOCK_Stream &new_stream,
                                  ACE_Accept_QoS_Params qos_params,
                                  ACE_Addr *remote_addr,
                                  ACE_Time_Value *timeout,
                                  int restart,
                                  int reset_new_handle) const
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::accept");
  shared_ssl_init_one (new_stream);
  if (acceptor_.accept (new_stream.peer (),
                        qos_params,
                        remote_addr,
                        timeout,
                        restart,
                        reset_new_handle) == -1) {
    return -1;
  }
  return shared_ssl_init_two (new_stream);
}

int
ACE_Secure_SOCK_Acceptor::get_local_addr (ACE_Addr &addr) const
{
  ACE_TRACE ("ACE_SECURE_SOCK_Acceptor::get_local_addr");
  return acceptor_.get_local_addr (addr);
}

void
ACE_Secure_SOCK_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_Secure_SOCK_Acceptor::dump");
  acceptor_.dump ();
}

#define HOME "./"
/* Make these what you want for cert & key files */
#define CERTF  HOME "jwh1.cert"
#define KEYF  HOME  "key"

#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2);}

int
ACE_Secure_SOCK_Acceptor::shared_ssl_init_one (ACE_Secure_SOCK_Stream& new_stream) const{
  SSL_load_error_strings ();
  SSL_library_init ();
  SSL_METHOD* meth = SSLv23_server_method ();
  SSL_CTX* ctx = SSL_CTX_new (meth);
  if (!ctx) {
    ERR_print_errors_fp (stderr);
    exit (2);
  }

  if (SSL_CTX_use_certificate_file (ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp (stderr);
    exit (3);
  }
  if (SSL_CTX_use_PrivateKey_file (ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp (stderr);
    exit(4);
  }

  if (!SSL_CTX_check_private_key (ctx)) {
    fprintf (stderr,"Private key does not match the certificate public key\n");
    exit (5);
  }

  new_stream.session (SSL_new (ctx));         CHK_NULL(new_stream.session ());
  return 0;
}

int
ACE_Secure_SOCK_Acceptor::shared_ssl_init_two (ACE_Secure_SOCK_Stream& new_stream) const{
  SSL_set_fd (new_stream.session (), new_stream.get_handle ());
  int err = SSL_accept (new_stream.session ());                        CHK_SSL (err);
  return 0;
}

#endif /* defined (ACE_HAS_OPENSSL) */
