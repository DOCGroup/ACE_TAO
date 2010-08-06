// -*- C++ -*-

#ifndef TAO_OBJECT_REFERENCE_FACTORY_H
#define TAO_OBJECT_REFERENCE_FACTORY_H

#include "ObjectReferenceFactoryC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class ObjectReferenceFactory
  : public virtual OBV_lwft::ObjectReferenceFactory
  , public virtual CORBA::DefaultValueRefCountBase
{
 public:

  /// Constructor
  ObjectReferenceFactory (
    PortableInterceptor::ObjectReferenceFactory * old_orf);

  /**
   * @name PortableInterceptor::ObjectReferenceFactory Methods
   *
   * Methods required by the
   * PortableInterceptor::ObjectReferenceFactory interface.
   */
  //@{
  virtual CORBA::Object_ptr make_object (
      const char *repository_id,
      const PortableInterceptor::ObjectId & id);
  //@}

protected:

  /// Destructor
  /**
   * Protected destructor to enforce proper memory management via
   * reference counting.
   */
  ~ObjectReferenceFactory (void);

private:

  /// The old ObjectReferenceFactory used to create object references.
  /**
   * This ObjectReferenceFactory will still be used when creating
   * object references.  However, it will be invoked through this
   * ObjectReferenceFactory.
   */
  PortableInterceptor::ObjectReferenceFactory_var old_orf_;

};


#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_OBJECT_REFERENCE_FACTORY_H */
