// -*- C++ -*-

//=============================================================================
/**
 * @file LB_ObjectReferenceFactory.h
 *
 * $Id$

 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_LB_OBJECT_REFERENCE_FACTORY_H
#define TAO_LB_OBJECT_REFERENCE_FACTORY_H

#include "ace/pre.h"

#include "orbsvcs/LB_ORTC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class TAO_LB_ObjectReferenceFactory
 *
 * @brief Implementation of the PortableInterceptor::ObjectReferenceFactory
 *        interface.
 *
 * This ObjectReferenceFactory creates an object group for a member of
 * the given repository ID (if instructed to do so), creates an
 * "unpublished" reference for that member, and adds it to the object
 * group via the LoadManager.
 */
class TAO_LB_ObjectReferenceFactory
  : public virtual CORBA::DefaultValueRefCountBase
  , public virtual OBV_TAO_LB::ObjectReferenceFactory
{
 public:

  /// Constructor
  TAO_LB_ObjectReferenceFactory (
    PortableInterceptor::ObjectReferenceFactory * old_orf,
    const char * repository_ids);

  /**
   * @name PortableInterceptor::ObjectReferenceFactory Methods
   *
   * Methods required by the
   * PortableInterceptor::ObjectReferenceFactory interface.
   */
  //@{
  virtual CORBA::Object_ptr make_object (
      const char * repository_id,
      const PortableInterceptor::ObjectId & id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management via
   * reference counting.
   */
  ~TAO_LB_ObjectReferenceFactory (void);

private:

  /// The old ObjectReferenceFactory used to create object references.
  /**
   * This ObjectReferenceFactory will be used when creating object
   * references for non-load balanced objects.
   */
  PortableInterceptor::ObjectReferenceFactory_var old_orf_;

  /// Space separated list of RepositoryIds corresponding to objects
  /// that will be load balanced.
  /**
   * @note The actual storage for this string is controlled by the
   *       TAO_LB_ORBInitializer.
   */
  const char * repository_ids_;

  /// Table that maps repository ID to object group reference.
  Table table_;

};


#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif  /* TAO_LB_OBJECT_REFERENCE_FACTORY_H */
