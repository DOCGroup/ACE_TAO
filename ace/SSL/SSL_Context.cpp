// -*- C++ -*-
// $Id$

//
// ========================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SSL_Context.cpp
//
// = AUTHOR
//    Chris Zimman
//    Carlos O'Ryan <coryan@ece.uciedu>
//    Ossama Othman <ossama@ece.uci.du>
//
// ========================================================================

#if defined (ACE_HAS_SSL)

#include "SSL_Context.h"
#include "sslconf.h"

#if !defined(__ACE_INLINE__)
#include "SSL_Context.i"
#endif /* __ACE_INLINE__ */

#include "ace/Synch.h"
#include "ace/Object_Manager.h"

#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/rand.h>


#ifdef ACE_HAS_THREADS
ACE_mutex_t * ACE_SSL_Context::lock_ = 0;
#endif  /* ACE_HAS_THREADS */

// @@ We really need a better seed value.  A seed value based on the
//    date and time, in combination with some other strings, may
//    suffice.
//      -Ossama
static const char rnd_seed[] = "string to make the random number generator think it has entropy";

int ACE_SSL_Context::library_init_count_ = 0;

ACE_SSL_Context::ACE_SSL_Context ()
  : context_ (0),
    mode_ (-1),
    default_verify_mode_ (SSL_VERIFY_NONE)
{
  ACE_SSL_Context::ssl_library_init ();
}

ACE_SSL_Context::~ACE_SSL_Context ()
{
  if (this->context_)
    {
      ::SSL_CTX_free(this->context_);
      this->context_ = 0;
    }
  ACE_SSL_Context::ssl_library_fini ();
}

void
ACE_SSL_Context::ssl_library_init ()
{
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex,
                     ace_ssl_mon,
                     *ACE_Static_Object_Lock::instance ()));

  if (ACE_SSL_Context::library_init_count_ == 0)
    {
      ::SSL_library_init ();
      ::SSL_load_error_strings ();
      ::SSLeay_add_ssl_algorithms ();

      // Seed the random number generator
      // @@ TODO: Need to pick a better seed value.
      ::RAND_seed (rnd_seed,
                   sizeof rnd_seed);

#ifdef ACE_HAS_THREADS
      int num_locks = ::CRYPTO_num_locks ();

      ACE_NEW (ACE_SSL_Context::lock_,
               ACE_mutex_t[num_locks]);

      for (int i = 0; i < num_locks; ++i)
        {
          // rwlock_init(&(ACE_SSL_Context::lock_[i]), USYNC_THREAD,
          // 0);
          if (ACE_OS::mutex_init(&(ACE_SSL_Context::lock_[i]),
                                 USYNC_THREAD,
                                 0) != 0)
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) ACE_SSL_Context::ssl_library_init - %p\n",
                        "mutex_init"));
        }

# if !defined (WIN32)
      // This call isn't necessary on some platforms.  See the CRYPTO
      // library's threads(3) man page for details.
      ::CRYPTO_set_id_callback (ACE_SSL_thread_id);
# endif  /* WIN32 */
      ::CRYPTO_set_locking_callback (ACE_SSL_locking_callback);
#endif  /* ACE_HAS_THREADS */
    }
  ACE_SSL_Context::library_init_count_++;
}

void
ACE_SSL_Context::ssl_library_fini ()
{
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex,
                     ace_ssl_mon,
                     *ACE_Static_Object_Lock::instance ()));

  ACE_SSL_Context::library_init_count_--;
  if (ACE_SSL_Context::library_init_count_ == 0)
    {
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
    {
      ::ERR_print_errors_fp (stderr);
      return -1;
    }

  this->mode_ = mode;

  const char *cert_file = ACE_OS::getenv (ACE_SSL_CERT_FILE_ENV);
  if (cert_file == 0)
    cert_file = ACE_DEFAULT_SSL_CERT_FILE;
  const char *cert_dir = ACE_OS::getenv (ACE_SSL_CERT_DIR_ENV);
  if (cert_dir == 0)
    cert_dir = ACE_DEFAULT_SSL_CERT_DIR;

  ::SSL_CTX_load_verify_locations (this->context_,
                                   cert_file,
                                   cert_dir);
  ::ERR_print_errors_fp (stderr);

  if (this->certificate_.type () != -1
      && ::SSL_CTX_use_certificate_file (this->context_,
                                         this->certificate_.file_name (),
                                         this->certificate_.type ()) <= 0)
    {
      // ERR_print_errors_fp (stderr);
      return -1;
    }
  if (this->private_key_.type () != -1
      && SSL_CTX_use_PrivateKey_file (this->context_,
                                      this->private_key_.file_name (),
                                      this->private_key_.type ()) <= 0)
    {
      // ERR_print_errors_fp (stderr);
      return -1;
    }

  if (!::SSL_CTX_check_private_key (this->context_))
    {
      // ACE_ERROR ((LM_ERROR, "Mismatch in key/certificate\n"));
      return -1;
    }
  return 0;
}

int
ACE_SSL_Context::get_mode (void) const
{
  return this->mode_;
}

int
ACE_SSL_Context::private_key (const char *file_name,
                              int type)
{
  if (this->private_key_.type () != -1)
    return 0;

  this->private_key_ = ACE_SSL_Data_File (file_name, type);

  if (this->context_ == 0)
    return 0;

  int status =
    ::SSL_CTX_use_PrivateKey_file (this->context_,
                                   this->private_key_.file_name (),
                                   this->private_key_.type ());
  return status;
}

int
ACE_SSL_Context::verify_private_key (void)
{
  this->check_context ();

  return ::SSL_CTX_check_private_key (this->context_);
}

int
ACE_SSL_Context::certificate (const char *file_name,
                              int type)
{
  if (this->certificate_.type () != -1)
    return 0;

  this->certificate_ = ACE_SSL_Data_File (file_name, type);

  if (this->context_ == 0)
    return 0;

  int status =
    ::SSL_CTX_use_certificate_file (this->context_,
                                    this->certificate_.file_name (),
                                    this->certificate_.type ());
  return status;
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

template class ACE_Singleton<ACE_SSL_Context,ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<ACE_SSL_Context,ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif	/* ACE_HAS_SSL */
