// -*- C++ -*-

//=============================================================================
/**
 * @file ObjectReferenceTemplate.h
 *
 * $Id$
 *
 * This is the implementation of the
 * PortableInterceptor::ObjectReferenceTemplate ValueType.
 *
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_OBJECT_REFERENCE_TEMPLATE_H
#define TAO_OBJECT_REFERENCE_TEMPLATE_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

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

/**
 * @class TAO_ObjectReferenceTemplate
 *
 * @brief Implementation of the PortableInterceptor::ObjectReferenceTemplate
 *        interface.
 */
class TAO_PortableServer_Export TAO_ObjectReferenceTemplate
  : public virtual OBV_TAO_Default_ORT::ObjectReferenceTemplate,
    public virtual CORBA::DefaultValueRefCountBase
{
public:

  /// Constructor
  TAO_ObjectReferenceTemplate (const char *server_id,
                               const char *orb_id,
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
  ~TAO_ObjectReferenceTemplate (void);

private:

  const char *server_id_;
  const char *orb_id_;
  PortableInterceptor::AdapterName_var adapter_name_;
  TAO_POA *poa_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "ObjectReferenceTemplate.inl"
#endif /* ! __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_REFERENCE_TEMPLATE_H */
