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

#include "pi_server_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "PI_Server.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{
  namespace Portable_Server
  {
    class POA_Current_Impl;

    class TAO_PI_Server_Export POA_Current
      : public PortableServer::Current,
        public TAO_Local_RefCounted_Object
    {
    public:
      /// Constructor
      POA_Current (void);

      /**
       * Returns the POA on which the current request is being invoked.
       * Can raise the @c NoContext exception if this function is
       * not invoked in the context of an upcall.
       */
      PortableServer::POA_ptr get_POA (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::Current::NoContext));

      /**
       * Returns the object id of the current request being invoked.  Can
       * raise the @c NoContext exception if this function is not
       * invoked in the context of an upcall.
       */
      PortableServer::ObjectId *get_object_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::Current::NoContext));

      /**
       * Returns a reference to the servant that hosts the object in whose
       * context it is called. If called outside the context of the POA
       * dispatched operation, a NoContext exception is raised
       */
      PortableServer::Servant get_servant (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::Current::NoContext));

      /// Returns the class that implements this interface.
      POA_Current_Impl *implementation (void);

      /// Sets the thread-specific pointer to the new POA Current state,
      /// returning a pointer to the existing POA Current state.
      POA_Current_Impl *implementation (POA_Current_Impl *new_current);
    };
  }
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "POA_Current.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_POA_CURRENT_H */
