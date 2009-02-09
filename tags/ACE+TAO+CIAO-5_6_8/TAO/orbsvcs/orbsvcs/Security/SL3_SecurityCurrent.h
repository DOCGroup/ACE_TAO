// -*- C++ -*-

//=============================================================================
/**
 * @file SL3_SecurityCurrent.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SL3_SECURITY_CURRENT_H
#define TAO_SL3_SECURITY_CURRENT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Security/security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Security/SL3_SecurityCurrent_Impl.h"

#include "orbsvcs/SecurityLevel3C.h"

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

namespace TAO
{
  namespace SL3
  {
    /**
     * @class SecurityCurrent
     *
     * @brief SecurityLevel3::SecurityCurrent implementation.
     *
     * Thread-specific information may be retrieved from the target
     * security service through this object.
     *
     * @note This SecurityCurrent implementation basically a variant
     *       of the bridge design pattern.  All operations are
     *       delegated on to concrete implementations.
     */
    class TAO_Security_Export SecurityCurrent
      : public virtual SecurityLevel3::SecurityCurrent,
        public virtual ::CORBA::LocalObject
    {
    public:

      /// Constructor
      SecurityCurrent (size_t tss_slot, TAO_ORB_Core * oc);

      /**
       * @name SecurityLevel3::SecurityCurrent Methods
       *
       * Methods required by the SecurityLevel3::SecurityCurrent
       * interface.
       */
      //@{
      virtual SecurityLevel3::ClientCredentials_ptr client_credentials ();

      virtual CORBA::Boolean request_is_local (void);
      //@}

      /// Return the TSS slot ID assigned to the "SecurityCurrent"
      /// object.
      /**
       * The concrete thread-specific storage SecurityCurrent
       * implementations will each use this slot ID.
       */
      size_t tss_slot (void) const;

    protected:

      /// Destructor
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~SecurityCurrent (void);

    private:

      /// Set the thread-specific storage
      /// SecurityLevel3::SecurityCurrent implementation.
      /**
       * The pointer is actually one to a concrete implementation provided
       * by the underlying security mechanism.  For example, SSLIOP
       * implements its own SSLIOP-specific operations corresponding
       * to the ones exposed by the SecurityCurrent interface.
       * Similarly, SECIOP would do the same.
       * @par
       * There is no function that places the implementation pointer
       * in TSS.  The underlying security mechanism does that.
       */
      SecurityCurrent_Impl * implementation (void);

    private:

      /**
       * @name Retricted Copying and Assignment
       *
       * Prevent copying through the copy constructor and the
       * assignment operator.
       */
      //@{
      SecurityCurrent (const SecurityCurrent &);
      void operator= (const SecurityCurrent &);
      //@}

    private:

      /// Thread-specific storage slot assigned to this object.
      const size_t tss_slot_;

      /// Pointer to the ORB Core corresponding to the ORB with which
      /// this object is registered.
      TAO_ORB_Core * const orb_core_;

    };

  } // End SL3 namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "orbsvcs/Security/SL3_SecurityCurrent.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SL3_SECURITY_CURRENT_H */
