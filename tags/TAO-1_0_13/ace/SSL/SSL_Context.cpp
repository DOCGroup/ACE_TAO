/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SSL_Context.cpp
//
// = AUTHOR
//    Chris Zimman
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

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
      // @@ What should we do here???
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<ACE_SSL_Context,ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Singleton<ACE_SSL_Context,ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif	/* ACE_HAS_SSL */
