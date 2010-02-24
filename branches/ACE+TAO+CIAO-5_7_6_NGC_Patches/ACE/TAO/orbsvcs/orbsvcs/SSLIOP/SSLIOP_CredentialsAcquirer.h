// -*- C++ -*-

//=============================================================================
/**
 * @file SSLIOP_CredentialsAcquirer.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_CREDENTIALS_ACQUIRER_H
#define TAO_SSLIOP_CREDENTIALS_ACQUIRER_H

#include /**/ "ace/pre.h"
#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Security/SL3_CredentialsCurator.h"

#include "orbsvcs/SSLIOPC.h"
#include "orbsvcs/SecurityLevel3C.h"

#include "tao/LocalObject.h"

#include <openssl/opensslconf.h>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/// Forward declarations for OpenSSL data structures.
extern "C"
{
  typedef struct x509_st X509;
  typedef struct evp_pkey_st EVP_PKEY;
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @class CredentialsAcquirer
     *
     * @brief SSLIOP-specific SecurityLevel3::CredentialsAcquirer
     *        implementation.
     *
     * This class generates SSLIOP-specific credentials.
     */
    class TAO_SSLIOP_Export CredentialsAcquirer
      : public virtual SecurityLevel3::CredentialsAcquirer,
        public virtual ::CORBA::LocalObject
    {
    public:

      /// Constructor
      CredentialsAcquirer (TAO::SL3::CredentialsCurator_ptr curator,
                           const CORBA::Any & acquisition_arguments);

      /**
       * @name SecurityLevel3::CredentialsAcquirer Methods
       *
       * Methods required by the SecurityLevel3::CredentialsAcquirer
       * interface.
       */
      //@{
      virtual char * acquisition_method (void);

      virtual SecurityLevel3::AcquisitionStatus current_status ();

      virtual CORBA::ULong nth_iteration (void);

      virtual CORBA::Any * get_continuation_data (void);

      virtual SecurityLevel3::AcquisitionStatus continue_acquisition (
          const CORBA::Any & acquisition_arguments);

      virtual SecurityLevel3::OwnCredentials_ptr get_credentials (
          CORBA::Boolean on_list);

      virtual void destroy (void);
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~CredentialsAcquirer (void);

    private:

      /// Verify that this CredentialsAcquirer object is still valid,
      /// i.e. hasn't been destroyed.
      void check_validity (void);

      /// Create an OpenSSL X.509 certificate data structure.
      static ::X509 * make_X509 (const ::SSLIOP::File &certificate);

      /// Create an OpenSSL EVP_PKEY key data structure.
      static ::EVP_PKEY * make_EVP_PKEY (const ::SSLIOP::File &key);

    private:

      /// Lock used for synchronization.
      TAO_SYNCH_MUTEX lock_;

      /// Reference to the TAO CredentialsCurator implementation.
      TAO::SL3::CredentialsCurator_var curator_;

      /// SSLIOP-specific credentials acquisition arguments.
      const CORBA::Any & acquisition_arguments_;

      /// Has this CredentialsAcquirer object completed credentials
      /// acquisition or been explicitly destroyed?
      bool destroyed_;

    };

  } // End SSLIOP namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CREDENTIALS_ACQUIRER_H */
