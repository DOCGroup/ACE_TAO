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

/**
 * @class TAO_ObjectReferenceTemplate_Adapter
 *
 * @brief Implementation of the PortableInterceptor::ObjectReferenceTemplate
 *        interface.
 */
class TAO_ORT_Export TAO_ObjectReferenceTemplate_Adapter_Impl
  : public TAO_ObjectReferenceTemplate_Adapter
{
public:
  TAO_ObjectReferenceTemplate_Adapter_Impl();

  /// Activate this adapter
  virtual int activate (const char *server_id,
                        const char *orb_id,
                        PortableInterceptor::AdapterName *adapter_name,
                        TAO_POA *poa
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Set a different ort_factory to be used.
  virtual int set_obj_ref_factory (PortableInterceptor::ObjectReferenceFactory *current_factory
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * @name PortableInterceptor::ObjectReferenceTemplate Methods
   *
   * Methods required by the
   * PortableInterceptor::ObjectReferenceTemplate ValueType.
   */
  //@{
  virtual char * server_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * orb_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::AdapterName * adapter_name (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /// Accessor methods to ObjectReferenceTemplate template
  virtual PortableInterceptor::ObjectReferenceTemplate * get_adapter_template (void);

  /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
  virtual PortableInterceptor::ObjectReferenceFactory * get_obj_ref_factory (void);

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
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
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
  ~TAO_ObjectReferenceTemplate_Adapter_Impl (void);

private:

  /// The ORT Template, this is the factory and its identify
  PortableInterceptor::ObjectReferenceTemplate_var ort_template_;

  /// The ORT Factory
  PortableInterceptor::ObjectReferenceFactory_var ort_factory_;

  /// TAO specific ORT Template
  TAO_ObjectReferenceTemplate* tao_ort_template;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate_Adapter_Impl.inl"
#endif /* ! __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_REFERENCE_TEMPLATE_ADAPTER_IMPL_H */
