// -*- C++ -*-

//=============================================================================
/**
 * @file ObjectReferenceTemplate_Adapter_Impl.h
 *
 * $Id$
 *
 * This is the implementation of the TAO_ObjectReferenceTemplate_Adapter
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_OBJECT_REFERENCE_TEMPLATE_ADAPTER_IMPL_H
#define TAO_OBJECT_REFERENCE_TEMPLATE_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "ort_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ObjectReferenceTemplate_Adapter.h"
#include "tao/ObjRefTemplate/ObjectReferenceTemplate_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_POA;

/// Local aliasing
// @@ Johnny see this trick to make things smaller.
namespace TAO_PI = PortableInterceptor;

namespace TAO
{
  /**
   * @class ObjectReferenceTemplate_Adapter_Impl
   *
   * @brief Implementation of the TAO_ObjectReferenceTemplate_Adapter
   */
  class TAO_ORT_Export ObjectReferenceTemplate_Adapter_Impl
    : public ObjectReferenceTemplate_Adapter
  {
  public:
    ObjectReferenceTemplate_Adapter_Impl (void);

    /// Activate this adapter
    virtual int activate (const char *server_id,
                          const char *orb_id,
                          PortableInterceptor::AdapterName *adapter_name,
                          TAO_POA *poa
                          ACE_ENV_ARG_DECL);

    /// Set a different ort_factory to be used.
    virtual int set_obj_ref_factory (
      PortableInterceptor::ObjectReferenceFactory *current_factory
      ACE_ENV_ARG_DECL);

    /**
     * @name PortableInterceptor::ObjectReferenceTemplate Methods
     *
     * Methods required by the
     * PortableInterceptor::ObjectReferenceTemplate ValueType.
     */
    // @@ Johnny, could you please either add a tao_* to the following
    // or document them that they are adapter methods. This could be
    // confusing.
    //@{
    virtual char * server_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual char * orb_id (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual TAO_PI::AdapterName * adapter_name (
        ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
    //@}

    /// Accessor methods to ObjectReferenceTemplate template
    virtual TAO_PI::ObjectReferenceTemplate *get_adapter_template (void);

    /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
    virtual TAO_PI::ObjectReferenceFactory * get_obj_ref_factory (void);

    /**
     * @name PortableInterceptor::ObjectReferenceFactory Methods
     *
     * Methods required by the
     * PortableInterceptor::ObjectReferenceFactory ValueType.
     */
    //@{
    virtual CORBA::Object_ptr make_object (
        const char * repository_id,
        const TAO_PI::ObjectId & id
        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
    //@}

    /// Set the underlying POA pointer.
    /**
     * Upon destruction of the POA that this ObjectReferenceTemplate is
     * associated with, the underlying POA pointer will be set to zero
     * to forcibly break all ties with the POA.  This is necessary to
     * prevent this ObjectReferenceTemplate from invoking the POA after
     * it has been destroyed.
     */
    void poa (TAO_POA * poa);

  protected:

    /// Destructor
    /**
     * Protected destructor to enforce proper memory management via
     * reference counting.
     */
    virtual ~ObjectReferenceTemplate_Adapter_Impl (void);

  private:

    /// The ORT Template, this is the factory and its identify
    PortableInterceptor::ObjectReferenceTemplate_var ort_template_;

    /// The ORT Factory
    PortableInterceptor::ObjectReferenceFactory_var ort_factory_;

    /// TAO specific ORT Template
    ObjectReferenceTemplate* tao_ort_template_;
  };

}
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate_Adapter_Impl.inl"
#endif /* ! __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_REFERENCE_TEMPLATE_ADAPTER_IMPL_H */
