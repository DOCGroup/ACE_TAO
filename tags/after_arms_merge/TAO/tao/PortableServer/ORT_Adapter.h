// -*- C++ -*-

//=============================================================================
/**
 *  @file    ORT_Adapter.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================
#ifndef TAO_ORT_ADAPTER_H
#define TAO_ORT_ADAPTER_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/SystemException.h"
#include "tao/PI_ForwardC.h"
#include "ace/Service_Object.h"
#include "ace/Array_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

namespace PortableInterceptor
{
  class ObjectReferenceTemplate;
  class ObjectReferenceFactory;
}

namespace PortableServer
{
  class POA;
  typedef POA* POA_ptr;
}

namespace TAO
{
  /**
   * @class ORT_Adapter
   *
   * @brief ORT_Adapter
   *
   * Class that offers an interface to the POA to load and manipulate
   * ORT library.
   */
  class TAO_PortableServer_Export ORT_Adapter
    : public ACE_Service_Object
  {
  public:
    /// Activate the ORT library
    virtual int activate (const char *server_id,
                          const char *orb_id,
                          PortableInterceptor::AdapterName *,
                          PortableServer::POA_ptr poa
                          ACE_ENV_ARG_DECL) = 0;

    /// Set a different ort_factory to be used.
    virtual int set_obj_ref_factory (
      PortableInterceptor::ObjectReferenceFactory *current_factory
      ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

    /// Accessor methods to ObjectReferenceTemplate template
    virtual PortableInterceptor::ObjectReferenceTemplate *
      get_adapter_template (void) = 0;

    /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
    virtual PortableInterceptor::ObjectReferenceFactory *
      get_obj_ref_factory (void) = 0;

    /// Release any references on @c ObjectReferenceTemplate @a t.
    virtual void release (
      PortableInterceptor::ObjectReferenceTemplate * t) = 0;

    /**
     * @name PortableInterceptor::ObjectReferenceFactory Methods
     *
     * Methods required by the
     * PortableInterceptor::ObjectReferenceFactory ValueType.
     *
     * @todo
     * @@ Johnny, we won't be needing the exception specification
     * below for long.  Once bug 1852 is fixed, we can the
     * exception specification and the "tao/SystemException.h"
     * include above.
     */
    //@{
    virtual CORBA::Object_ptr make_object (
      const char * repository_id,
      const PortableInterceptor::ObjectId & id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    )) = 0;
    //@}
  };

 typedef
  ACE_Array_Base<PortableInterceptor::ObjectReferenceTemplate*> ORT_Array;

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ORT_ADAPTER_H */
