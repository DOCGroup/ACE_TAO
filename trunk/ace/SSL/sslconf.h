// -*- C++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//     ACE_SSL
//
// = FILENAME
//     sslconf.h
//
// = AUTHOR
//     Carlos O'Ryan <coryan@ece.uci.edu>
//
// ============================================================================


#ifndef ACE_SSLCONF_H
#define ACE_SSLCONF_H

#include "ace/pre.h"

#if !defined (ACE_DEFAULT_SSL_CERT_FILE)
#define ACE_DEFAULT_SSL_CERT_FILE "/etc/ssl/cert.pem"
#endif /* ACE_DEFAULT_SSL_CERT_FILE */

#if !defined (ACE_DEFAULT_SSL_CERT_DIR)
#define ACE_DEFAULT_SSL_CERT_DIR "/etc/ssl/certs"
#endif /* ACE_DEFAULT_SSL_CERT_DIR */

#if !defined (ACE_SSL_CERT_FILE_ENV)
#define ACE_SSL_CERT_FILE_ENV "SSL_CERT_FILE"
#endif /* ACE_SSL_CERT_FILE_ENV */

#if !defined (ACE_SSL_CERT_DIR_ENV)
#define ACE_SSL_CERT_DIR_ENV  "SSL_CERT_DIR"
#endif /* ACE_SSL_CERT_DIR_ENV */

#include "ace/post.h"

#endif /* ACE_SSLCONF_H */

