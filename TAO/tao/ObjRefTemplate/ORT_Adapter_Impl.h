// -*- C++ -*-

//=============================================================================
/**
 * @file ORT_Adapter_Impl.h
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
    int activate (const char *server_id,
                          const char *orb_id,
                          PortableInterceptor::AdapterName *adapter_name,
                          PortableServer::POA_ptr poa) override;

    /// Accessor methods to ObjectReferenceTemplate template
    PortableInterceptor::ObjectReferenceTemplate *get_adapter_template (void) override;

    /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
    PortableInterceptor::ObjectReferenceFactory * get_obj_ref_factory (void) override;

    /// Set a different ort_factory to be used.
    int set_obj_ref_factory (
      PortableInterceptor::ObjectReferenceFactory * current_factory) override;

    /**
     * @see @c TAO::ORT_Adapter for details.
     */
    void release (PortableInterceptor::ObjectReferenceTemplate * ort) override;

    /**
     * @name Adapter methods for PortableInterceptor::ObjectReferenceTemplate
     * Methods
     */
    //@{
    virtual char * tao_server_id (void);

    virtual char * tao_orb_id (void);

    virtual PortableInterceptor::AdapterName * tao_adapter_name (void);
    //@}

    /**
     * @name PortableInterceptor::ObjectReferenceFactory Methods
     *
     * Methods required by the
     * PortableInterceptor::ObjectReferenceFactory ValueType.
     */
    //@{
    CORBA::Object_ptr make_object (
        const char * repository_id,
        const PortableInterceptor::ObjectId & id) override;
    //@}

  private:

    /// The ORT Template, this is the factory and its identity.
    PortableInterceptor::ObjectReferenceTemplate_var ort_template_;

    /// The ORT Factory.
    PortableInterceptor::ObjectReferenceFactory_var ort_factory_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ORT_ADAPTER_IMPL_H */
