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

#include "security_export.h"
#include "orbsvcs/SecurityLevel2C.h"

/**
 * @class TAO_Security_Current_Impl
 *
 * @brief Base class for the TSS portion of any underlying security
 *        mechanism.
 *
 * This class provides the same interface as the
 * SecurityLevel2::Current object.  However, it is not derived from
 * that interface since we need to explicitly avoid virtual
 * inheritance so that it is safe to store subclasses in a "void *"
 * and later cast that pointer back to the subclass pointer type.
 */
class TAO_Security_Export TAO_Security_Current_Impl
{
public:

  /// Destructor.
  virtual ~TAO_Security_Current_Impl (void);

  /**
   * @name SecurityLevel1::Current Methods
   *
   * These methods are founds in the SecurityLevel1::Current
   * interface.
   */
  //@{
  /// Return the security attributes corresponding to the types in the
  /// given attribute type list associated with the current request.
  virtual Security::AttributeList * get_attributes (
      const Security::AttributeTypeList & attributes
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
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
  virtual SecurityLevel2::ReceivedCredentials_ptr received_credentials (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  //@}

  /// Return the unique tag that identifies the concrete subclass.
  virtual CORBA::ULong tag (void) const = 0;

};

#include /**/ "ace/post.h"

#endif  /* TAO_SECURITY_CURRENT_IMPL_H */
