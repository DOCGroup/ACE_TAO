// -*- C++ -*-

// ===================================================================
/**
 *  @file   SL2_EstablishTrustPolicy.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_ESTABLISH_TRUST_POLICY_H
#define TAO_ESTABLISH_TRUST_POLICY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"

#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Security
  {

    /**
     * @class EstablishTrustPolicy
     *
     * @brief Implementation of the
     *        SecurityLevel2::EstablishTrustPolicy.
     *
     * This policy can be used to enable or disable establishment of
     * trust in the client or the target or both on a per-object
     * basis.  For example, it can be set as a policy override using
     * the standard CORBA::Object::_set_policy_overrides() method.
     @par
     * This policy can be created by using the
     * CORBA::ORB::create_policy() method by passing it the
     * Security::SecEstablishTrustPolicy policy type, and the
     *
     * appropriate Security::EstablishTrust structure (inserted into a
     * CORBA::Any).
     */
    class EstablishTrustPolicy
      : public virtual SecurityLevel2::EstablishTrustPolicy,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor
      EstablishTrustPolicy (const ::Security::EstablishTrust &trust);

      /**
       * @name CORBA::Policy Methods
       */
      //@{
      virtual CORBA::PolicyType policy_type (void);

      virtual CORBA::Policy_ptr copy (void);

      virtual void destroy (void);
      //@}

      /// Return the "establish trust" value associated with this
      /// policy.
      virtual ::Security::EstablishTrust trust (void);

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~EstablishTrustPolicy (void);

    private:

      /// Quality of protection which can be specified for an object
      /// reference and used to protect messages.
      ::Security::EstablishTrust const trust_;

    };

  } // End Security namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_ESTABLISH_TRUST_POLICY_H */
