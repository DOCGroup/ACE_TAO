// -*- C++ -*-

// ===================================================================
/**
 *  @file   Security_Current_Impl.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SECURITY_CURRENT_IMPL_H
#define TAO_SECURITY_CURRENT_IMPL_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Security/security_export.h"
#include "orbsvcs/SecurityLevel2C.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Security
  {
    /**
     * @class Current_Impl
     *
     * @brief Base class for the TSS portion of any underlying
     *        security mechanism.
     *
     * This class provides the same interface as the
     * SecurityLevel3::Current object.  However, it is not derived
     * from that interface since we need to explicitly avoid virtual
     * inheritance so that it is safe to store subclasses in a "void
     * *" and later cast that pointer back to the subclass pointer
     * type.
     */
    class TAO_Security_Export Current_Impl
    {
    public:

      /// Destructor.
      virtual ~Current_Impl (void);

      /**
       * @name SecurityLevel1::Current Methods
       *
       * These methods are founds in the SecurityLevel1::Current
       * interface.
       */
      //@{
      /// Return the security attributes corresponding to the types in
      /// the given attribute type list associated with the current
      /// request.
      virtual ::Security::AttributeList * get_attributes (
          const ::Security::AttributeTypeList & attributes)
        = 0;
      //@}

      /**
       * @name SecurityLevel2::Current Methods
       *
       * These methods are founds in the SecurityLevel2::Current
       * interface.
       */
      //@{
      /// Return the Credentials received from the client associate with
      /// the current request.
      virtual SecurityLevel2::ReceivedCredentials_ptr received_credentials ()
        = 0;
      //@}

      /// Return the unique tag that identifies the concrete subclass.
      virtual CORBA::ULong tag (void) const = 0;

    };

  }  // End Security namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SECURITY_CURRENT_IMPL_H */
