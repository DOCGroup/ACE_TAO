/*
** $Id$
**
** This is the Thread-per-connection logging server example from Chapter 7.
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS_NS_string.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/os_include/os_netdb.h"
#include "Reactor_Logging_Server_Adapter.h"
#include "TPC_Logging_Server.h"
#include "TPCLS_export.h"

int TPC_Logging_Handler::open (void *) {
  static const ACE_TCHAR LOGFILE_SUFFIX[] = ACE_TEXT (".log");
  ACE_TCHAR filename[MAXHOSTNAMELEN + sizeof (LOGFILE_SUFFIX)];
  ACE_INET_Addr logging_peer_addr;

  peer ().get_remote_addr (logging_peer_addr);
  logging_peer_addr.get_host_name (filename, MAXHOSTNAMELEN);
  ACE_OS::strcat (filename, LOGFILE_SUFFIX);

  ACE_FILE_Connector connector;
  connector.connect (log_file_,
                     ACE_FILE_Addr (filename),
                     0, // No timeout.
                     ACE_Addr::sap_any, // Ignored.
                     0, // Don't try to reuse the addr.
                     O_RDWR | O_CREAT | O_APPEND,
                     ACE_DEFAULT_FILE_PERMS);

  logging_handler_.peer ().set_handle (peer ().get_handle ());

  return activate (THR_NEW_LWP | THR_DETACHED);
}


#if !defined (TPC_CERTIFICATE_FILENAME)
#  define TPC_CERTIFICATE_FILENAME "tpc-cert.pem"
#endif /* !TPC_CERTIFICATE_FILENAME */
#if !defined (TPC_KEY_FILENAME)
#  define TPC_KEY_FILENAME "tpc-key.pem"
#endif /* !TPC_KEY_FILENAME */

int TPC_Logging_Acceptor::open
    (const ACE_SOCK_Acceptor::PEER_ADDR &local_addr,
           ACE_Reactor *reactor,
           int flags, int use_select, int reuse_addr) {
  if (PARENT::open (local_addr, reactor, flags,
                    use_select, reuse_addr) != 0)
    return -1;
  OpenSSL_add_ssl_algorithms ();
  ssl_ctx_ = SSL_CTX_new (SSLv3_server_method ());
  if (ssl_ctx_ == 0) return -1;

  if (SSL_CTX_use_certificate_file (ssl_ctx_,
                                    TPC_CERTIFICATE_FILENAME,
                                    SSL_FILETYPE_PEM) <= 0
      || SSL_CTX_use_PrivateKey_file (ssl_ctx_,
                                      TPC_KEY_FILENAME,
                                      SSL_FILETYPE_PEM) <= 0
      || !SSL_CTX_check_private_key (ssl_ctx_))
    return -1;
  ssl_ = SSL_new (ssl_ctx_);
  return ssl_ == 0 ? -1 : 0;
}


int TPC_Logging_Acceptor::handle_close (ACE_HANDLE h,
                                        ACE_Reactor_Mask mask) {
  PARENT::handle_close (h, mask);
  delete this;
  return 0;
}


int TPC_Logging_Acceptor::accept_svc_handler
    (TPC_Logging_Handler *sh) {
  if (PARENT::accept_svc_handler (sh) == -1) return -1;
  SSL_clear (ssl_);  // Reset for new SSL connection.
#if defined (ACE_WIN32)
  // ACE_WIN32 is the only platform where ACE_HANDLE is not an int.
  // See ace/config-lite.h for the typedefs.
  SSL_set_fd (ssl_, reinterpret_cast<int> (sh->get_handle ()));
#else
  SSL_set_fd (ssl_, sh->get_handle ());
#endif /* ACE_WIN32 */

  SSL_set_verify
    (ssl_,
     SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
     0);
  if (SSL_accept (ssl_) == -1
      || SSL_shutdown (ssl_) == -1) return -1;
  return 0;
}

typedef Reactor_Logging_Server_Adapter<TPC_Logging_Acceptor>
        TPC_Logging_Server;

ACE_FACTORY_DEFINE (TPCLS, TPC_Logging_Server)

