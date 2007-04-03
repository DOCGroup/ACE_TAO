// -*- C++ -*-

//=============================================================================
/**
 * @file SL3_PolicyFactory.h
 *
 * $Id$
 *
 * PolicyFactory implementation for the SecurityLevel3 policies.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SL3_POLICY_FACTORY_H
#define TAO_SL3_POLICY_FACTORY_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
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
       * @class olicyFactory
       *
       * @brief SecurityLevel3 PolicyFactory
       *
       * PolicyFactory for all SecurityLevel3 policies.
       */
      class PolicyFactory
        : public virtual PortableInterceptor::PolicyFactory,
          public virtual TAO_Local_RefCounted_Object
      {
      public:

        /**
         * @name Methods Required by the PolicyFactory Interface
         *
         * These are methods that must be implemented since they are
         * pure virtual in the abstract base class.  They are the
         * canonical methods required for all PolicyFactory
         * sub-classes.
         */
        //@{

        /// Construct a Test::Policy object as a test.
        virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                                 const CORBA::Any & value);

        //@}

      };

  }  // End SL3 namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_SL3_POLICY_FACTORY_H */
