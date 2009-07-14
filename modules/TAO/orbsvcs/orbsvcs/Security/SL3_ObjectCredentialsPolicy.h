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
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    class ObjectCredentialsPolicy
      : public virtual SecurityLevel3::ObjectCredentialsPolicy,
        public virtual ::CORBA::LocalObject
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
      virtual SecurityLevel3::OwnCredentialsList * creds_list ();

      virtual CORBA::PolicyType policy_type (void);

      virtual CORBA::Policy_ptr copy (void);

      virtual void destroy (void);
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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SL3_OBJECT_CREDENTIALS_POLICY_H */
