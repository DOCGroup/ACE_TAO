// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ObjectReferenceTemplate_Adapter.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================
#ifndef TAO_OBJECTREFERENCETEMPLATE_ADAPTER_H
#define TAO_OBJECTREFERENCETEMPLATE_ADAPTER_H

#include /**/ "ace/pre.h"
#include "ace/Array_Base.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"
#include "tao/PI_ForwardC.h"

#include "portableserver_export.h"

class TAO_POA;

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

/**
 * @class TAO_ObjectReferenceTemplate_Adapter
 *
 * @brief TAO_ObjectReferenceTemplate_Adapter.
 *
 * Class that offers an interface to the POA to load and manipulate
 * ORT library.
 */
class TAO_PortableServer_Export TAO_ObjectReferenceTemplate_Adapter
  : public ACE_Service_Object
{
public:

  /// The virtual destructor
  virtual ~TAO_ObjectReferenceTemplate_Adapter (void);

  /// Activate the ORT library
  virtual int activate (const char *server_id,
                        const char *orb_id,
                        PortableInterceptor::AdapterName *,
                        TAO_POA *poa
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Set a different ort_factory to be used.
  virtual int set_obj_ref_factory (
      PortableInterceptor::ObjectReferenceFactory *current_factory
      ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Set the underlying POA pointer.
  /**
   * Upon destruction of the POA that this ObjectReferenceTemplate is
   * associated with, the underlying POA pointer will be set to zero
   * to forcibly break all ties with the POA.  This is necessary to
   * prevent this ObjectReferenceTemplate from invoking the POA after
   * it has been destroyed.
   */
  virtual void poa (TAO_POA * poa) = 0;

  /// Accessor methods to ObjectReferenceTemplate template
  virtual PortableInterceptor::ObjectReferenceTemplate *
      get_adapter_template (void) = 0;

  /// Accessor methods to PortableInterceptor::ObjectReferenceFactory
  virtual PortableInterceptor::ObjectReferenceFactory *
      get_obj_ref_factory (void) = 0;

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
    )) = 0;
  //@}
};

typedef
ACE_Array_Base<PortableInterceptor::ObjectReferenceTemplate*>
    TAO_ObjectReferenceTemplate_Array;

#include /**/ "ace/post.h"
#endif /* TAO_OBJECTREFERENCETEMPLATE_ADAPTER_H */
