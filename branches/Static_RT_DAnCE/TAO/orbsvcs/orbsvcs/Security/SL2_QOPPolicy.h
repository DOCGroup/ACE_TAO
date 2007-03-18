// -*- C++ -*-

// ===================================================================
/**
 *  @file   SL2_QOPPolicy.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_SL2_QOP_POLICY_H
#define TAO_SL2_QOP_POLICY_H

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
     * @class QOPPolicy
     *
     * @brief Implementation of the SecurityLevel2::QOPPolicy
     *        interface.
     *
     * This policy can be used to affect the quality of protection
     * (QoP) for invocation on a per-object basis.  For example, it
     * can be set as a policy override using the standard
     * CORBA::Object::_set_policy_overrides() method.
     * @par
     * This policy can be created by using the
     * CORBA::ORB::create_policy() method by passing it the
     * Security::SecQOPPolicy policy type, and the appropriate
     * Security::QOP enumeration (inserted into a CORBA::Any).
     */
    class QOPPolicy
      : public virtual SecurityLevel2::QOPPolicy,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor
      QOPPolicy (::Security::QOP qop);

      /**
       * @name CORBA::Policy Methods
       */
      //@{
      virtual CORBA::PolicyType policy_type (void);

      virtual CORBA::Policy_ptr copy (void);

      virtual void destroy (void);
      //@}

      /// Return the Quality-of-Protection value associated with this
      /// policy.
      virtual ::Security::QOP qop (void);

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechansim.
       */
      ~QOPPolicy (void);

    private:

      /// Quality of protection which can be specified for an object
      /// reference and used to protect messages.
      ::Security::QOP const qop_;

    };

  } // End Security namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SL2_QOP_POLICY_H */
