// -*- C++ -*-

//=============================================================================
/**
 *  @file    ObjectReferenceTemplate_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_OBJECTREFERENCETEMPLATE_ADAPTER_FACTORY_IMPL_H
#define TAO_OBJECTREFERENCETEMPLATE_ADAPTER_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ort_export.h"
#include "tao/PortableServer/ObjectReferenceTemplate_Adapter_Factory.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_ObjectReferenceTemplate_Adapter_Factory_Impl
 *
 * @brief TAO_ObjectReferenceTemplate_Adapter_Factory_Impl
 *
 * Class that creates instances of TAO_ObjectReferenceTemplate_Adapter
 * (one per POA). This is the derived class that contains the actual
 * implementation.
 */
class TAO_ORT_Export TAO_ObjectReferenceTemplate_Adapter_Factory_Impl
  : public TAO_ObjectReferenceTemplate_Adapter_Factory
{
public:
  virtual ~TAO_ObjectReferenceTemplate_Adapter_Factory_Impl (void);

  virtual TAO_ObjectReferenceTemplate_Adapter * create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      TAO_ObjectReferenceTemplate_Adapter * adapter
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Used to force the initialization of the code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_ObjectReferenceTemplate_Adapter_Factory_Impl)
ACE_FACTORY_DECLARE (TAO_ORT, TAO_ObjectReferenceTemplate_Adapter_Factory_Impl)

#if defined (ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_ORTFactory_Initializer =
  &TAO_ObjectReferenceTemplate_Adapter_Factory_Impl::Initializer;

#else

static int
TAO_Requires_ORTFactory_Initializer =
  TAO_ObjectReferenceTemplate_Adapter_Factory_Impl::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"

#endif /* TAO_OBJECTREFERENCETEMPLATE_ADAPTER_FACTORY_IMPL_H */
