
// -*- C++ -*-

//=============================================================================
/**
 * @file SL3_ObjectCredentialsPolicy.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SL3_OBJECT_CREDENTIALS_POLICY_H
#define TAO_SL3_OBJECT_CREDENTIALS_POLICY_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel3C.h"

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{
  namespace SL3
  {
    /**
     * @class ObjectCredentialsPolicy
     *
     * @brief POA-specific Policy containing server's "own"
     *        credentials.
     *
     * This policy may be passed in the PolicyList argument of
     * PortableServer::POA::create_POA() method.  Targets under that
     * POA will have the credentials contained within this Policy
     * associated with them.
     */
    class TAO_Security_Export ObjectCredentialsPolicy
      : public virtual SecurityLevel3::ObjectCredentialsPolicy,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor
      ObjectCredentialsPolicy (
        const SecurityLevel3::OwnCredentialsList & creds);

      /**
       * @name SecurityLevel3::ObjectCredentialsPolicy Methods
       *
       * Methods required by the
       * SecurityLevel3::ObjectCredentialsPolicy interface.
       */
      //@{
      virtual SecurityLevel3::OwnCredentialsList * creds_list (
          ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
      //@}

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~ObjectCredentialsPolicy (void);

    private:

      /// List of POA-specific OwnCredentials.
      SecurityLevel3::OwnCredentialsList creds_list_;

    };

  } // End SL3 namespace

}  // End TAO namespace


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SL3_OBJECT_CREDENTIALS_POLICY_H */
