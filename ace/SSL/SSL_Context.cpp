// -*- C++ -*-

#include "SSL_Context.h"

#include "sslconf.h"

#if !defined(__ACE_INLINE__)
#include "SSL_Context.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Synch.h"
#include "ace/Object_Manager.h"
#include "ace/Log_Msg.h"

#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/rand.h>

ACE_RCSID (ACE_SSL,
           SSL_Context,
           "$Id$")

#ifdef ACE_HAS_THREADS
ACE_mutex_t * ACE_SSL_Context::lock_ = 0;
#endif  /* ACE_HAS_THREADS */


int ACE_SSL_Context::library_init_count_ = 0;

ACE_SSL_Context::ACE_SSL_Context (void)
  : context_ (0),
    mode_ (-1),
    default_verify_mode_ (SSL_VERIFY_NONE),
    have_ca_ (0)
{
  ACE_SSL_Context::ssl_library_init ();
}

ACE_SSL_Context::~ACE_SSL_Context (void)
{
  if (this->context_)
    {
      ::SSL_CTX_free (this->context_);
      this->context_ = 0;
    }

  ACE_SSL_Context::ssl_library_fini ();
}

void
ACE_SSL_Context::ssl_library_init (void)
{
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex,
                     ace_ssl_mon,
                     *ACE_Static_Object_Lock::instance ()));

  if (ACE_SSL_Context::library_init_count_ == 0)
    {
      // Initialize the locking callbacks before initializing anything
      // else.
#ifdef ACE_HAS_THREADS
      int num_locks = ::CRYPTO_num_locks ();

      ACE_NEW (ACE_SSL_Context::lock_,
               ACE_mutex_t[num_locks]);

      for (int i = 0; i < num_locks; ++i)
        {
          // rwlock_init(&(ACE_SSL_Context::lock_[i]), USYNC_THREAD,
          // 0);
          if (ACE_OS::mutex_init (&(ACE_SSL_Context::lock_[i]),
                                  USYNC_THREAD) != 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) ACE_SSL_Context::ssl_library_init ")
                        ACE_TEXT ("- %p\n"),
                        ACE_TEXT ("mutex_init")));
        }

# if !defined (WIN32)
      // This call isn't necessary on some platforms.  See the CRYPTO
      // library's threads(3) man page for details.
      ::CRYPTO_set_id_callback (ACE_SSL_thread_id);
# endif  /* WIN32 */
      ::CRYPTO_set_locking_callback (ACE_SSL_locking_callback);
#endif  /* ACE_HAS_THREADS */


      ::SSL_library_init ();
      ::SSL_load_error_strings ();
      ::SSLeay_add_ssl_algorithms ();


      // Seed the random number generator.  Note that the random
      // number generator can be seeded more than once to "stir" its
      // state.

#ifdef WIN32
      // Seed the random number generator by sampling the screen.
      ::RAND_screen ();
#endif  /* WIN32 */

#if OPENSSL_VERSION_NUMBER >= 0x00905100L
      // OpenSSL < 0.9.5 doesn't have EGD support.

      const char *egd_socket_file =
        ACE_OS::getenv (ACE_SSL_EGD_FILE_ENV);

      if (egd_socket_file != 0)
        (void) this->egd_file (egd_socket_file);
#endif  /* OPENSSL_VERSION_NUMBER */

      const char *rand_file =
        ACE_OS::getenv (ACE_SSL_RAND_FILE_ENV);

      if (rand_file != 0)
        (void) this->seed_file (rand_file);

      // Initialize the mutexes that will be used by the SSL and
      // crypto library.

    }

  ACE_SSL_Context::library_init_count_++;
}

void
ACE_SSL_Context::ssl_library_fini (void)
{
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex,
                     ace_ssl_mon,
                     *ACE_Static_Object_Lock::instance ()));

  ACE_SSL_Context::library_init_count_--;
  if (ACE_SSL_Context::library_init_count_ == 0)
    {
      ::ERR_free_strings ();
      ::EVP_cleanup ();

      // Clean up the locking callbacks after everything else has been
      // cleaned up.
#ifdef ACE_HAS_THREADS
      int num_locks = ::CRYPTO_num_locks ();

      ::CRYPTO_set_locking_callback (0);
      for (int i = 0; i < num_locks; ++i)
        ACE_OS::mutex_destroy (&(ACE_SSL_Context::lock_[i]));

      delete [] ACE_SSL_Context::lock_;
#endif  /* ACE_HAS_THREADS */
    }
}

int
ACE_SSL_Context::set_mode (int mode)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex,
                            ace_ssl_mon,
                            *ACE_Static_Object_Lock::instance (),
                            -1));

  if (this->context_ != 0)
    return -1;

  SSL_METHOD *method = 0;

  switch (mode)
    {
    case ACE_SSL_Context::SSLv2_client:
      method = ::SSLv2_client_method ();
      break;
    case ACE_SSL_Context::SSLv2_server:
      method = ::SSLv2_server_method ();
      break;
    case ACE_SSL_Context::SSLv2:
      method = ::SSLv2_method ();
      break;
    case ACE_SSL_Context::SSLv3_client:
      method = ::SSLv3_client_method ();
      break;
    case ACE_SSL_Context::SSLv3_server:
      method = ::SSLv3_server_method ();
      break;
    case ACE_SSL_Context::SSLv3:
      method = ::SSLv3_method ();
      break;
    case ACE_SSL_Context::SSLv23_client:
      method = ::SSLv23_client_method ();
      break;
    case ACE_SSL_Context::SSLv23_server:
      method = ::SSLv23_server_method ();
      break;
    case ACE_SSL_Context::SSLv23:
      method = ::SSLv23_method ();
      break;
    case ACE_SSL_Context::TLSv1_client:
      method = ::TLSv1_client_method ();
      break;
    case ACE_SSL_Context::TLSv1_server:
      method = ::TLSv1_server_method ();
      break;
    case ACE_SSL_Context::TLSv1:
      method = ::TLSv1_method ();
      break;
    default:
      method = ::SSLv3_method ();
      break;
    }

  this->context_ = ::SSL_CTX_new (method);
  if (this->context_ == 0)
    return -1;

  this->mode_ = mode;

  // Load the trusted certificate authority (default) certificate
  // locations. But do not return -1 on error, doing so confuses CTX
  // allocation (severe error) with the less important loading of CA
  // certificate location error.  If it is important for your
  // application then call ACE_SSL_Context::have_trusted_ca(),
  // immediately following this call to set_mode().
  (void) this->load_trusted_ca ();

  return 0;
}

int
ACE_SSL_Context::load_trusted_ca (const char* ca_file, const char* ca_dir)
{
  this->check_context ();

  if (ca_file == 0)
    {
      // Use the default environment settings.
      ca_file = ACE_OS::getenv (ACE_SSL_CERT_FILE_ENV);
      if (ca_file == 0)
        ca_file = ACE_DEFAULT_SSL_CERT_FILE;
    }

  if (ca_dir == 0)
    {
      // Use the default environment settings.
      ca_dir = ACE_OS::getenv (ACE_SSL_CERT_DIR_ENV);
      if (ca_dir == 0)
        ca_dir = ACE_DEFAULT_SSL_CERT_DIR;
    }

  // NOTE: SSL_CTX_load_verify_locations() returns 0 on error.
  if (::SSL_CTX_load_verify_locations (this->context_,
                                       ca_file,
                                       ca_dir) <= 0)
    {
      if (ACE::debug ())
        ACE_SSL_Context::report_error ();
      return -1;
    }
  else
    {
      this->have_ca_++;

      // for TLS/SSL servers scan all certificates in ca_file and list
      // then as acceptable CAs when requesting a client certificate.
      if (mode_ == SSLv23
          || mode_ == SSLv23_server
          || mode_ == TLSv1
          || mode_ == TLSv1_server
          || mode_ == SSLv3
          || mode_ == SSLv3_server
          || mode_ == SSLv2
          || mode_ == SSLv2_server)
        {
          STACK_OF (X509_NAME) * cert_names;
          cert_names = ::SSL_CTX_get_client_CA_list(this->context_);

          if (cert_names == 0)
            {
              // Set the first certificate authorith list.
              cert_names = ::SSL_load_client_CA_file (ca_file);
              if (cert_names != 0 )
                ::SSL_CTX_set_client_CA_list (this->context_,
                                              cert_names);
            }
          else
            {
              // Add new certificate names to the list.
              if (!::SSL_add_file_cert_subjects_to_stack (cert_names,
                                                          ca_file))
                cert_names = 0;
            }

          if (cert_names == 0)
            {
              if (ACE::debug ())
                ACE_SSL_Context::report_error ();
              return -1;
            }

          // @todo
          // If warranted do the same for ca_dir when the function
          // SSL_add_dir_cert_subjects_to_stack() is portable to
          // WIN32, VMS, MAC_OS_pre_X (nb. it is not defined for those
          // platforms by OpenSSL).
        }
    }

  return 0;
}


int
ACE_SSL_Context::private_key (const char *file_name,
                              int type)
{
  if (this->private_key_.type () != -1)
    return 0;

  this->check_context ();

  this->private_key_ = ACE_SSL_Data_File (file_name, type);

  if (::SSL_CTX_use_PrivateKey_file (this->context_,
                                     this->private_key_.file_name (),
                                     this->private_key_.type ()) <= 0)
    return -1;
  else
    return this->verify_private_key ();
}

int
ACE_SSL_Context::verify_private_key (void)
{
  this->check_context ();

  return (::SSL_CTX_check_private_key (this->context_) <= 0 ? -1 : 0);
}

int
ACE_SSL_Context::certificate (const char *file_name,
                              int type)
{
  if (this->certificate_.type () != -1)
    return 0;

  this->certificate_ = ACE_SSL_Data_File (file_name, type);

  this->check_context ();

  if (::SSL_CTX_use_certificate_file (this->context_,
                                      this->certificate_.file_name (),
                                      this->certificate_.type ()) <= 0)
    return -1;
  else
    return 0;
}

void
ACE_SSL_Context::set_verify_peer (int strict, int once, int depth)
{
  this->check_context ();

  // Setup the peer verififcation mode.

  int verify_mode = SSL_VERIFY_PEER;
  if (once)
    verify_mode |= SSL_VERIFY_CLIENT_ONCE;
  if (strict)
    verify_mode |= SSL_VERIFY_FAIL_IF_NO_PEER_CERT;

  // set the default verify mode
  this->default_verify_mode (verify_mode);

  // Set the max certificate depth but later let the verify_callback
  // catch the depth error by adding one to the required depth.
  if (depth > 0)
    ::SSL_CTX_set_verify_depth (this->context_, depth + 1);
}


int
ACE_SSL_Context::random_seed (const char * seed)
{
  ::RAND_seed (seed, ACE_OS::strlen (seed));

#if OPENSSL_VERSION_NUMBER >= 0x00905100L
  // RAND_status() returns 1 if the PRNG has enough entropy.
  return (::RAND_status () == 1 ? 0 : -1);
#else
  return 0;  // Ugly, but OpenSSL <= 0.9.4 doesn't have RAND_status().
#endif  /* OPENSSL_VERSION_NUMBER >= 0x00905100L */
}

int
ACE_SSL_Context::egd_file (const char * socket_file)
{
#if OPENSSL_VERSION_NUMBER < 0x00905100L
  // OpenSSL < 0.9.5 doesn't have EGD support.
  ACE_UNUSED_ARG (socket_file);
  ACE_NOTSUP_RETURN (-1);
#else
  // RAND_egd() returns the amount of entropy used to seed the random
  // number generator.  The actually value should be greater than 16,
  // i.e. 128 bits.
  if (::RAND_egd (socket_file) > 0)
    return 0;
  else
    return -1;
#endif  /* OPENSSL_VERSION_NUMBER >= 0x00905100L */
}

int
ACE_SSL_Context::seed_file (const char * seed_file, long bytes)
{
  // RAND_load_file() returns the number of bytes used to seed the
  // random number generator.
  if (::RAND_load_file (seed_file, bytes) > 0)
    return 0;
  else
    return -1;
}

void
ACE_SSL_Context::report_error (unsigned long error_code)
{
  if (error_code == 0)
    return;

  char error_string[256];

  (void) ::ERR_error_string (error_code, error_string);

  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACE_SSL (%P|%t) error code: %u - %s\n"),
              error_code,
              error_string));
}

void
ACE_SSL_Context::report_error (void)
{
  unsigned long error = ::ERR_get_error ();
  ACE_SSL_Context::report_error (error);
  ACE_OS::last_error (error);
}

int
ACE_SSL_Context::dh_params (const char *file_name,
                            int type)
{
  if (this->dh_params_.type () != -1)
    return 0;

  this->dh_params_ = ACE_SSL_Data_File (file_name, type);

  this->check_context ();

  {
    // For now we only support PEM encodings
    if (this->dh_params_.type () != SSL_FILETYPE_PEM)
      return -1;

    // Swiped from Rescorla's examples and the OpenSSL s_server.c app
    DH *ret=0;
    FILE *dhfp = 0;

    if ((dhfp = ACE_OS::fopen (this->dh_params_.file_name (), "r")) == NULL)
      return -1;

    ret = PEM_read_DHparams (dhfp, NULL, NULL, NULL);
    ACE_OS::fclose (dhfp);
    if(::SSL_CTX_set_tmp_dh (this->context_, ret) < 0)
      return -1;
    DH_free (ret);
  }

  return 0;
}

// ****************************************************************

#ifdef ACE_HAS_THREADS

void
ACE_SSL_locking_callback (int mode,
                          int type,
                          const char * /* file */,
                          int /* line */)
{
  // #ifdef undef
  //   fprintf(stderr,"thread=%4d mode=%s lock=%s %s:%d\n",
  //           CRYPTO_thread_id(),
  //           (mode&CRYPTO_LOCK)?"l":"u",
  //           (type&CRYPTO_READ)?"r":"w",file,line);
  // #endif
  //   /*
  //     if (CRYPTO_LOCK_SSL_CERT == type)
  //     fprintf(stderr,"(t,m,f,l) %ld %d %s %d\n",
  //     CRYPTO_thread_id(),
  //     mode,file,line);
  //   */
  if (mode & CRYPTO_LOCK)
    ACE_OS::mutex_lock (&(ACE_SSL_Context::lock_[type]));
  else
    ACE_OS::mutex_unlock (&(ACE_SSL_Context::lock_[type]));
}



unsigned long
ACE_SSL_thread_id (void)
{
  return (unsigned long) ACE_OS::thr_self ();
}
#endif  /* ACE_HAS_THREADS */

// ****************************************************************



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<ACE_SSL_Context, ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<ACE_SSL_Context, ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
