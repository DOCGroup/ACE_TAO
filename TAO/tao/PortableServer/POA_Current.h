// -*- C++ -*-

//=============================================================================
/**
 *  @file    POA_Current.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_POA_CURRENT_H
#define TAO_POA_CURRENT_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PS_CurrentC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class POA_Current_Impl;

    class TAO_PortableServer_Export POA_Current
      : public PortableServer::Current
      , public TAO_Local_RefCounted_Object
    {
    public:

      /**
       * Returns the POA on which the current request is being invoked.
       * Can raise the @c NoContext exception if this function is
       * not invoked in the context of an upcall.
       */
      PortableServer::POA_ptr get_POA (void);

      /**
       * Returns the object id of the current request being invoked.  Can
       * raise the @c NoContext exception if this function is not
       * invoked in the context of an upcall.
       */
      PortableServer::ObjectId *get_object_id (void);

      /**
       * This operation returns a locally manufactured reference to the object
       * in the context of which it is called. If called outside the context
       * of a POA dispatched operation, a NoContext exception is raised.
       * @note This reference is not guaranteed to be identical to the original
       * reference the client used to make the invocation, and calling the
       * Object::is_equivalent operation to compare the two references may not
       * necessarily return true.
       */
      CORBA::Object_ptr get_reference (void);

      /**
       * Returns a reference to the servant that hosts the object in whose
       * context it is called. If called outside the context of the POA
       * dispatched operation, a NoContext exception is raised
       */
      PortableServer::Servant get_servant (void);

      /// Returns the class that implements this interface.
      POA_Current_Impl *implementation (void);

      /// Sets the thread-specific pointer to the new POA Current state,
      /// returning a pointer to the existing POA Current state.
      POA_Current_Impl *implementation (POA_Current_Impl *new_current);
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_POA_CURRENT_H */
