// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   ace
//
// = FILENAME
//   SSL_Context.h
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_SSL_CONTEXT_H
#define ACE_SSL_CONTEXT_H

#include "ace/SString.h"

#if defined (ACE_HAS_SSL)

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "ace/Synch.h"

#include <openssl/ssl.h>

// @@ We need another export for this library!
class ACE_Export ACE_SSL_Data_File
{
public:
  ACE_SSL_Data_File ();
  // Default constructor

  ACE_SSL_Data_File (const char *file_name,
                     int type = SSL_FILETYPE_PEM);
  // Contructor from a file name and the file type.

  // Default dtor, cpy ctor and operator=

  const char *file_name (void) const;
  // The file name

  int type (void) const;
  // The type

private:
  ACE_CString file_name_;
  // The file name

  int type_;
  // The type, used by the SSL library to parse the file contents.
};

// ****************************************************************

class ACE_Export ACE_SSL_Context
{
  // = TITLE
  //   A wrapper for the ACE_SSL_Context class.
  //
  // = DESCRIPTION
  //   This class provides a wrapper for the SSL_CTX data structure.
  //   Since most applications have a single SSL_CTX structure, this
  //   class can be used as a singleton.

public:

  enum {
    INVALID_METHOD = -1,
    SSLv2_client = 1,
    SSLv2_server,
    SSLv2,
    SSLv3_client,
    SSLv3_server,
    SSLv3,
    SSLv23_client,
    SSLv23_server,
    SSLv23,
    TLSv1_client,
    TLSv1_server,
    TLSv1
  };

  ACE_SSL_Context ();
  // Constructor

  ~ACE_SSL_Context (void);
  // Destructor

  static ACE_SSL_Context *instance (void);
  // The Singleton context, the SSL components use the singleton if
  // nothing else is available.

  int set_mode (int mode = ACE_SSL_Context::SSLv3);
  // Set the CTX mode.  The mode can be set only once, afterwards the
  // function has no effect and returns -1.
  // Once the mode is set the underlying SSL_CTX is initialized and
  // the class can be used.
  // If the mode is not set, the the class automatically initializes
  // itself to the default mode.

  int get_mode (void) const;
  // @@ John, you need to document each function or at least each
  // group of functions. Also remember to follow the ACE guidelines,
  // this includes:
  // - a space between the function name and the '(' starting its
  // argument list.
  // - a single space after the return value
  // - Using const where appropriate
  //
  // You may not like the style (i don't) but it is more important
  // that we all use the same than keeping each one of us happy.

  SSL_CTX *context (void);
  // Get the SSL context

  int private_key_type (void) const;
  const char *private_key_file_name (void) const;
  // Get the file name and file format used for the private key

  int private_key (const char *file_name,
                   int type = SSL_FILETYPE_PEM);
  // Set the private key file.

  int verify_private_key (void);
  // Verify if the private key is valid

  int certificate_type (void) const;
  const char *certificate_file_name (void) const;
  // Get the file name and file format used for the certificate file

  int certificate (const char *file_name,
                   int type = SSL_FILETYPE_PEM);
  // Set the certificate file.

  void default_verify_mode (int mode);
  int default_verify_mode (void) const;
  // Set and query the default verify mode for this context, it is
  // inherited by all the ACE_SSL objects created using the context.
  // It can be overriden on a per-ACE_SSL object.

private:
  void check_context (void);
  // Verify if the context has been initialized or not.

  void ssl_library_init ();
  void ssl_library_fini ();
  // @@ More to document

private:
  // @@ Carlos, I protected this variable with an ACE_GUARD, just like
  //    what we do for the orb_init_count_ variable in
  //    tao/ORB.cpp.   The code isn't pretty but it should suffice
  //    until the SSL context is stored in a Singleton.
  //       -Ossama

  SSL_CTX *context_;
  // The SSL_CTX structure

  int mode_;
  // Cache the mode so we can answer fast

  ACE_SSL_Data_File private_key_;
  ACE_SSL_Data_File certificate_;
  // The private key and certificate file

  int default_verify_mode_;
  // The default verify mode.

  static int library_init_count_;
  // @@ This should also be done with a singleton, otherwise it is not
  // thread safe and/or portable to some weird platforms...
};

#if defined(__ACE_INLINE__)
#include "SSL_Context.i"
#endif /* __ACE_INLINE__ */

#endif	/* ACE_HAS_SSL */

#endif	/* ACE_SSL_CONTEXT_H */
