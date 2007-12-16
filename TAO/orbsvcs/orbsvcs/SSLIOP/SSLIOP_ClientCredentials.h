// -*- C++ -*-

//=============================================================================
/**
 * @file SSLIOP_ClientCredentials.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_CLIENT_CREDENTIALS_H
#define TAO_SSLIOP_CLIENT_CREDENTIALS_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Credentials.h"
#include "orbsvcs/SSLIOP/SSLIOP_SSL.h"

#include "orbsvcs/SecurityLevel3C.h"

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @class ClientCredentials
     *
     * @brief
     *
     *
     */
    class ClientCredentials
      : public virtual SecurityLevel3::ClientCredentials,
        public virtual SSLIOP_Credentials
    {
    public:

      /// Constructor
      ClientCredentials (::X509 * cert, ::EVP_PKEY * evp, ::SSL * ssl);

      /**
       * @name SecurityLevel3::Credentials Methods
       *
       * Methods required by the SecurityLevel3::Credentials
       * interface.
       */
      //@{
      virtual SecurityLevel3::CredentialsType creds_type ();
      //@}

      /**
       * @name SecurityLevel3::ClientCredentials Methods
       *
       * Methods required by the SecurityLevel3::ClientCredentials
       * interface.
       */
      //@{
      virtual char * context_id (void);

      virtual SecurityLevel3::Principal * client_principal ();

      virtual SecurityLevel3::StatementList * client_supporting_statements ();

      virtual SecurityLevel3::ResourceNameList * client_restricted_resources ();

      virtual SecurityLevel3::Principal * target_principal ();

      virtual SecurityLevel3::StatementList * target_supporting_statements ();

      virtual SecurityLevel3::ResourceNameList * target_restricted_resources ();

      virtual SecurityLevel3::OwnCredentials_ptr parent_credentials ();

      virtual CORBA::Boolean client_authentication (void);

      virtual CORBA::Boolean target_authentication (void);

      virtual CORBA::Boolean confidentiality (void);

      virtual CORBA::Boolean integrity (void);
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~ClientCredentials (void);

    private:

      /// Reference to the OpenSSL @c SSL data structure associated
      /// with the current security context (e.g. SSL connection).
      TAO::SSLIOP::SSL_var ssl_;

    };

  } // End SSLIOP namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CLIENT_CREDENTIALS_H */
