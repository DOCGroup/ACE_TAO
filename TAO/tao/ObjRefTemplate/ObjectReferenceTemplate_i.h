// -*- C++ -*-

//=============================================================================
/**
 * @file ObjectReferenceTemplate_i.h
 *
 * $Id$
 *
 * This is the implementation of the
 * PortableInterceptor::ObjectReferenceTemplate ValueType.
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_OBJECT_REFERENCE_TEMPLATE_I_H
#define TAO_OBJECT_REFERENCE_TEMPLATE_I_H

#include /**/ "ace/pre.h"

#include "tao/ObjRefTemplate/ort_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Default_ORTC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_POA;

// @@ Johnny, I am just wondering whether this class is even needed?
// Why do we need to provide a default implementation? Does the spec
// call fo it. I know why we did before.. Thoughts??
namespace TAO
{
  /**
   * @class TAO_ObjectReferenceTemplate
   *
   * @brief Implementation of the PortableInterceptor::ObjectReferenceTemplate
   *        interface.
   */
  class TAO_ORT_Export ObjectReferenceTemplate
    : public virtual OBV_TAO_Default_ORT::ObjectReferenceTemplate,
      public virtual CORBA::DefaultValueRefCountBase
  {
  public:
    /// Constructor
    ObjectReferenceTemplate (const char *server_id,
                                 const char *orb_id,
                                 PortableInterceptor::AdapterName *adapter_name,
                                 TAO_POA *poa);

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
    ~ObjectReferenceTemplate (void);

  private:
    const char *server_id_;
    const char *orb_id_;
    PortableInterceptor::AdapterName_var adapter_name_;
    TAO_POA *poa_;

  };
}
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate_i.inl"
#endif /* ! __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_REFERENCE_TEMPLATE_I_H */
