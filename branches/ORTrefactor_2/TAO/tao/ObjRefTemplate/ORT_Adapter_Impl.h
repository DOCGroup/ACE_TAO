// -*- C++ -*-

//=============================================================================
/**
 * @file ORT_Adapter_Impl.h
 *
 * $Id$
 *
 * This is the implementation of the TAO::ObjectReferenceTemplate_Adapter
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ORT_ADAPTER_IMPL_H
#define TAO_ORT_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "ort_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ORT_Adapter.h"
#include "tao/ObjRefTemplate/ObjectReferenceTemplate_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_POA;

namespace TAO
{
  /**
   * @class ObjectReferenceTemplate_Adapter_Impl
   *
   * @brief Implementation of the TAO::ObjectReferenceTemplate_Adapter
   */
  class TAO_ORT_Export ORT_Adapter_Impl
    : public ORT_Adapter
  {
  public:
    ORT_Adapter_Impl (void);

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

    /// Accessor methods to ObjectReferenceTemplate template
    virtual PortableInterceptor::ObjectReferenceTemplate *get_adapter_template (void);

    /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
    virtual PortableInterceptor::ObjectReferenceFactory * get_obj_ref_factory (void);

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
    virtual ~ORT_Adapter_Impl (void);

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

#include /**/ "ace/post.h"

#endif /* TAO_ORT_ADAPTER_IMPL_H */
