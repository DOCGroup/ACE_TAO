// -*- C++ -*-
//=============================================================================
/**
 *  @file    SSL_Context.h
 *
 *  @author Carlos O'Ryan <coryan@ece.uci.edu>
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_SSL_CONTEXT_H
#define ACE_SSL_CONTEXT_H

#include /**/ "ace/pre.h"

#include "SSL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

#ifdef ACE_HAS_THREADS
# include "ace/Synch_Traits.h"
#endif  /* ACE_HAS_THREADS */

#include <openssl/ssl.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_INET_Addr;

class ACE_SSL_Export ACE_SSL_Data_File
{
public:
  /// Default constructor
  ACE_SSL_Data_File ();

  /// Constructor from a file name and the file type.
  ACE_SSL_Data_File (const char *file_name,
                     int type = SSL_FILETYPE_PEM);

  /// The file name
  const char *file_name () const;

  /// The type
  int type () const;

private:
  /// The file name
  ACE_CString file_name_;

  /// The type, used by the SSL library to parse the file contents.
  int type_;
};

// ****************************************************************

// NOTE: Some compilers will issue warnings if the
// the correct type of function pointer (i.e. extern "C" ) is not stored/used
// of the form:
// Warning (Anachronism): Formal argument callback of type
//   extern "C" int(*)(int,x509_store_ctx_st*) in call to
//   SSL_CTX_set_verify(ssl_ctx_st*, int, extern "C" int(*)(int,x509_store_ctx_st*))
//   is being passed int(*)(int,x509_store_ctx_st*)
// when C library routines are passed CallBack functions pointers that are
// actually C++ functions.
//
// Unfortunatly you can not specify extern "C" linkage anywhere inside a class
// declaration or inside a function prototype for individual parameters. I.e:
//   class { extern "C" int (*callback_) (int, void *); };
// to store a function pointer as a data member of the class is illegal as is:
//   void function (extern "C" int (*callback) (int, void *);
// to declare function (or a class member) that takes a extern "C" function
// pointer as a parameter.
//
// Since we need an extern "C" function pointer as a parameter to be stored
// in the class and handled by member functions, we are forced to declare
// a typedef of that extern "C" function pointer that we can then use.
// Again unfortunatly you also are not allowed to simply add the extern "C"
// to the typedef itself, instead you have to place the typedef declaration
// inside an extern "C" block, thus:

extern "C" {
  typedef int (*extern_C_CallBackVerify_t) (int, X509_STORE_CTX *);
}

/**
 * @class ACE_SSL_Context
 *
 * @brief A wrapper for the OpenSSL SSL_CTX related functions.
 *
 * This class provides a wrapper for the SSL_CTX data structure.
 * Since most applications have a single SSL_CTX structure, this class
 * can be used as a singleton.
 */
class ACE_SSL_Export ACE_SSL_Context
{
public:
#ifdef ACE_HAS_THREADS
  typedef ACE_SYNCH_MUTEX lock_type;
#endif  /* ACE_HAS_THREADS */

  enum {
    INVALID_METHOD = -1,
    /* these negotiate highest available SSL/TLS version */
    SSLv23_client,
    SSLv23_server,
    SSLv23
  };

  /// Constructor
  ACE_SSL_Context ();

  /// Destructor
  ~ACE_SSL_Context ();

  /// The Singleton context, the SSL components use the singleton if
  /// nothing else is available.
  static ACE_SSL_Context *instance ();

  /// Explicitly delete the Singleton context.
  static void close ();

  /**
   * Set the CTX mode.  The mode can be set only once, afterwards the
   * function has no effect and returns -1.
   * Once the mode is set the underlying SSL_CTX is initialized and
   * the class can be used.
   * If the mode is not set, then the class automatically initializes
   * itself to the default mode.
   */
  int set_mode (int mode = ACE_SSL_Context::SSLv23);

  int get_mode () const;

  /// Get the SSL context
  SSL_CTX *context ();

  /// Get the file name and file format used for the private key
  int private_key_type () const;
  const char *private_key_file_name () const;

  /// Set the private key file.
  /**
   * @note This method should only be called after a certificate has
   *       been set since key verification is performed against the
   *       certificate, among other things.
   */
  int private_key (const char *file_name, int type = SSL_FILETYPE_PEM);

  /// Verify that the private key is valid.
  /**
   * @note This method should only be called after a certificate has
   *       been set since key verification is performed against the
   *       certificate, among other things.
   */
  int verify_private_key ();

  /// Get the file name and file format used for the certificate file
  int certificate_type () const;
  const char *certificate_file_name () const;

  /// Set the certificate file.
  int certificate (const char *file_name,
                   int type = SSL_FILETYPE_PEM);

  /// Load certificate from memory rather than a file.
  int certificate (X509* cert);

  /// Loads certificate chain file_name into ctx. Certificates must be
  /// sorted starting with the subject's certificate (actual client or
  /// server certificate), followed by ordered intermediate CA certificates.
  /// E.g. Let's Encrypt's intermediate certificate chain.
  int certificate_chain (const char *file_name, int type = SSL_FILETYPE_PEM);

  /// Parse the string and filter crypto versions accordingly
  int filter_versions (const char *filter);

  /// verify the peer cert matches the host
  bool check_host (const ACE_INET_Addr& host, SSL * peerssl);

  /**
   *  Load the location of the trusted certification authority
   *  certificates.  Note that CA certificates are stored in PEM format
   *  as a sequence of certificates in @a ca_file or as a set of
   *  individual certificates in @a ca_dir (or both).
   *
   *  Note this method is called by set_mode() to load the default
   *  environment settings for @a ca_file and @a ca_dir, if any. This
   *  allows for automatic service configuration (and backward
   *  compatibility with previous versions).
   *
   *  Note that the underlying SSL function will add valid file and
   *  directory names to the load location lists maintained as part of
   *  the SSL_CTX table.  It therefore doesn't make sense to keep a
   *  copy of the file and path name of the most recently added
   *  @a ca_file or @a ca_path.
   *
   *  @param[in] ca_file           CA file pathname. Passed to
   *                               @c SSL_CTX_load_verify_locations() if not
   *                               0. If 0, behavior depends on the value of
   *                               @a use_env_defaults.
   *  @param[in] ca_dir            CA directory pathname. Passed to
   *                               @c SSL_CTX_load_verify_locations() if not
   *                               0. If 0, behavior depends on the value of
   *                               @a use_env_defaults.
   *  @param[in] use_env_defaults  If false, the specified @a ca_file argument
   *                               is passed to
   *                               @c SSL_CTX_load_verify_locations(),
   *                               regardless of its value.
   *                               If true (the default), additional defaults
   *                               can be applied to either @a ca_file,
   *                               @a ca_dir, or both. The following
   *                               additional defaults are applied when the
   *                               @a ca_file argument is 0:
   *                                - The @c SSL_CERT_FILE environment variable
   *                                  will be queried for a file name to use as
   *                                  the @a ca_file argument. The environment
   *                                  variable name to query can be changed by
   *                                  supplying a @c ACE_SSL_CERT_FILE_ENV
   *                                  configuration item when building ACE.
   *                                - If there is no @c SSL_CERT_FILE in the
   *                                  current environment, the file specified
   *                                  by the @c ACE_DEFAULT_SSL_CERT_FILE ACE
   *                                  configuration item will be used. The
   *                                  default value is "cert.pem" on Windows
   *                                  and "/etc/ssl/cert.pem" on all other
   *                                  platforms.
   *                               The following additional defaults are
   *                               applied when the @a ca_dir argument is 0:
   *                                - The @c SSL_CERT_DIR environment variable
   *                                  will be queried for a file name to use as
   *                                  the @a ca_dir argument. The environment
   *                                  variable name to query can be changed by
   *                                  supplying a @c ACE_SSL_CERT_DIR_ENV
   *                                  configuration item when building ACE.
   *                                - If there is no @c SSL_CERT_DIR in the
   *                                  current environment, the directory
   *                                  specified by the @c
   *                                  ACE_DEFAULT_SSL_CERT_DIR ACE
   *                                  configuration item will be used. The
   *                                  default value is "certs" on Windows
   *                                  and "/etc/ssl/certs" on all other
   *                                  platforms.
   *
   *  @return 0 for success or -1 on error.
   *
   *  @see OpenSSL manual SSL_CTX_load_verify_locations(3) for a
   *  detailed description of the CA file and directory requirements
   *  and processing.
   */
  int load_trusted_ca (const char* ca_file = 0,
                       const char* ca_dir = 0,
                       bool use_env_defaults = true);

  /**
   *  Test whether any CA locations have been successfully loaded and
   *  return the number of successful attempts.
   *
   *  @retval >0  The number of successful CA load attempts.
   *  @retval  0  If all CA load attempts have failed.
   */
  int have_trusted_ca () const;

  /**
   *  @todo Complete this documentation where elipses(...) are used
   *
   *  @doc Use this method when certificate chain verification is
   *  required.  The default server behaviour is SSL_VERIFY_NONE
   *  i.e. client certicates are requested for verified. This method
   *  can be used to configure server to request client certificates
   *  and perform the certificate verification. If <strict> is set
   *  true the client connection is rejected when certificate
   *  verification fails.  Otherwise the session is accepted with a
   *  warning, which is the default behaviour.  If <once> is set true
   *  (default), certificates are requested only once per session.
   *  The last parameter <depth> can be used to set the verification
   *  depth.
   *
   *  Note for verification to work correctly there should be a valid
   *  CA name list set using load_trusted_ca().
   *
   *  @see OpenSSL documentation of SSL_CTX_set_verify(3) for details of
   *  the verification process.
   *
   *  @see OpenSSL documentation ... set_verify_depth(3) ...
   *
   *  Note that this method overrides the use of the
   *  default_verify_mode() method.
   */
  void set_verify_peer (int strict = 0, int once = 1, int depth = 0);

  /// TODO: a implementation that will lookup the CTX table for the list
  /// of files and paths etc.
  /// Query the location of trusted certification authority
  /// certificates.
  // const char* ca_file_name() const;
  // const char* ca_dir_name() const;

  /**
   * Set and query the default verify mode for this context, it is
   * inherited by all the ACE_SSL objects created using the context.
   * It can be overriden on a per-ACE_SSL object.
   */
  void default_verify_mode (int mode);
  int default_verify_mode () const;

  /**
   * Set and query the default verify callback for this context, it is
   * inherited by all the ACE_SSL objects created using the context.
   * It can be overriden on a per-ACE_SSL object.
   */
  void default_verify_callback (extern_C_CallBackVerify_t);
  extern_C_CallBackVerify_t  default_verify_callback () const;

  /**
   * @name OpenSSL Random Number Generator Seed Related Methods
   *
   * These are methods that can be used to seed OpenSSL's
   * pseudo-random number generator.  These methods can be called more
   * than once.
   */
  //@{
  /// Seed the underlying random number generator.  This value should
  /// have at least 128 bits of entropy.
  static int random_seed (const char * seed);

  /// Set the Entropy Gathering Daemon (EGD) UNIX domain socket file to
  /// read random seed values from.
  static int egd_file (const char * socket_file);

  /**
   * Set the file that contains the random seed value state, and the
   * amount of bytes to read.  "-1" bytes causes the entire file to be
   * read.
   */
  static int seed_file (const char * seed_file, long bytes = -1);
  //@}

  /// Print SSL error corresponding to the given error code.
  static void report_error (unsigned long error_code);

  /// Print the last SSL error for the current thread.
  static void report_error ();

  /**
   * @name Diffie-Hellman (DH) Parameters
   *
   * When using DSS-based certificates, Diffie-Hellman keys need to be
   * exchanged.  These must be provided in the form of DH key
   * generation parameters loaded in, or as fixed keys hardcoded into
   * the code itself.  ACE_SSL supports loaded parameters.
   *
   */
  //@{
  /**
   * Load Diffie-Hellman parameters from file_name.  The specified file can be
   * a standalone file containing only DH parameters (e.g., as created
   * by <code>openssl dhparam</code>), or it can be a certificate which has
   * a PEM-encoded set of DH params concatenated on to i.
   */
  int dh_params (const char *file_name, int type = SSL_FILETYPE_PEM);
  const char *dh_params_file_name () const;
  int dh_params_file_type () const;
  //@}

private:
  /// Verify if the context has been initialized or not.
  void check_context ();

  /// @@ More to document
  void ssl_library_init ();
  void ssl_library_fini ();

  ACE_SSL_Context (const ACE_SSL_Context &) = delete;
  ACE_SSL_Context & operator= (const ACE_SSL_Context &) = delete;

private:
  /// The SSL_CTX structure
  SSL_CTX *context_;

  /// Cache the mode so we can answer fast
  int mode_;

  /// The private key, certificate, and Diffie-Hellman parameters files
  ACE_SSL_Data_File private_key_;
  ACE_SSL_Data_File certificate_;
  ACE_SSL_Data_File dh_params_;

  /// The default verify mode.
  int default_verify_mode_;

  /// The default verify callback.
  extern_C_CallBackVerify_t  default_verify_callback_;

  /// count of successful CA load attempts
  int have_ca_;

#if defined(ACE_HAS_THREADS) && (OPENSSL_VERSION_NUMBER < 0x10100000L)
  /// Array of mutexes used internally by OpenSSL when the SSL
  /// application is multithreaded.
  static lock_type * locks_;
#endif /* ACE_HAS_THREADS && OPENSSL_VERSION_NUMBER < 0x10100000L */
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "SSL_Context.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* ACE_SSL_CONTEXT_H */
