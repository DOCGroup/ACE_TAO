// ================================================================
/**
 *  @file   Object_Loader.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
// ================================================================

#ifndef TAO_OBJECT_LOADER_H
#define TAO_OBJECT_LOADER_H
#include "ace/pre.h"

#include "tao/Exception.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

/**
 * @class TAO_Object_Loader
 *
 * @brief A class to dynamically load object implementations into an
 * ORB.
 *
 * Many services and components of the ORB can be dynamically
 * loaded, often these components are returned to the application via
 * an object reference (CORBA::Object_ptr).  This class is used to
 * dynamically load such components, and encapsulate the creation of
 * the object reference.
 *
 */
class TAO_Export TAO_Object_Loader : public ACE_Service_Object
{
public:
  /// The destructor
  virtual ~TAO_Object_Loader (void);

  /**
   * Create and activate a new object into the orb.
   * This method cannot throw any exception, but it can return a nil
   * object to indicate an error condition.
   */
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           char *argv []
                                           TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
};

#if defined (__ACE_INLINE__)
# include "Object_Loader.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_OBJECT_LOADER_H */
