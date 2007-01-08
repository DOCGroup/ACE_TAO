// -*- C++ -*-

//=============================================================================
/**
 * @file ORT_Adapter_Impl.h
 *
 * $Id$
 *
 * This is the implementation of the TAO::ORT_Adapter
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ORT_ADAPTER_IMPL_H
#define TAO_ORT_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/ObjRefTemplate/ort_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ORT_Adapter.h"
#include "tao/ObjRefTemplate/ObjectReferenceTemplate_i.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class ObjectReferenceTemplate_Adapter_Impl
   *
   * @brief Implementation of the TAO::ORT_Adapter
   */
  class ORT_Adapter_Impl
    : public ORT_Adapter
  {
  public:
    /// Activate this adapter
    virtual int activate (const char *server_id,
                          const char *orb_id,
                          PortableInterceptor::AdapterName *adapter_name,
                          PortableServer::POA_ptr poa
                          ACE_ENV_ARG_DECL);

    /// Accessor methods to ObjectReferenceTemplate template
    virtual PortableInterceptor::ObjectReferenceTemplate *get_adapter_template (void);

    /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
    virtual PortableInterceptor::ObjectReferenceFactory * get_obj_ref_factory (void);

    /// Set a different ort_factory to be used.
    virtual int set_obj_ref_factory (
      PortableInterceptor::ObjectReferenceFactory * current_factory
      ACE_ENV_ARG_DECL);

    /**
     * @see @c TAO::ORT_Adapter for details.
     */
    virtual void release (PortableInterceptor::ObjectReferenceTemplate * ort);

    /**
     * @name Adapter methods for PortableInterceptor::ObjectReferenceTemplate
     * Methods
     */
    //@{
    virtual char * tao_server_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual char * tao_orb_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual PortableInterceptor::AdapterName * tao_adapter_name (
        ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
    //@}

    /**
     * @name PortableInterceptor::ObjectReferenceFactory Methods
     *
     * Methods required by the
     * PortableInterceptor::ObjectReferenceFactory ValueType.
     */
    //@{
    virtual CORBA::Object_ptr make_object (
        const char * repository_id,
        const PortableInterceptor::ObjectId & id
        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
    //@}

  private:

    /// The ORT Template, this is the factory and its identity.
    PortableInterceptor::ObjectReferenceTemplate_var ort_template_;

    /// The ORT Factory.
    PortableInterceptor::ObjectReferenceFactory_var ort_factory_;

  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_ORT_ADAPTER_IMPL_H */
