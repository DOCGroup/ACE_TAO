// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSL_Asynch_BIO.h
 *
 *  $Id$
 *
 *  @author Alexander Libman <alibman@baltimore.com>
 *  @author Ossama Othman <ossama@uci.edu>
 *
 */
//=============================================================================

#ifndef ACE_SSL_ASYNCH_BIO_H
#define ACE_SSL_ASYNCH_BIO_H

#include /**/ "ace/pre.h"

#include "SSL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <openssl/bio.h>

#if OPENSSL_VERSION_NUMBER > 0x0090581fL && ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))

/**
 * @name OpenSSL BIO Helper Methods for use with ACE's Asynchronous
 *       SSL I/O support.
 */
//@{
extern "C"
{
  int  ACE_Asynch_BIO_write (BIO *pBIO, const char *buf, int len);
  int  ACE_Asynch_BIO_read  (BIO *pBIO, char *buf, int len);
  int  ACE_Asynch_BIO_puts  (BIO *pBIO, const char *str);
  long ACE_Asynch_BIO_ctrl  (BIO *pBIO, int cmd, long arg1, void *arg2);
  int  ACE_Asynch_BIO_new   (BIO *pBIO);
  int  ACE_Asynch_BIO_free  (BIO *pBIO);
}
//@}

#endif  /* OPENSSL_VERSION_NUMBER > 0x0090581fL (ACE_WIN32 ||
           ACE_HAS_AIO_CALLS) */

#include /**/ "ace/post.h"

#endif  /* ACE_SSL_ASYNCH_BIO_H */
