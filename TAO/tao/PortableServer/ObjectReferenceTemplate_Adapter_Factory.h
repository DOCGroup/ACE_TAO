// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ObjectReferenceTemplate_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_OBJECTREFERENCETEMPLATE_ADAPTER_FACTORY_H
#define TAO_OBJECTREFERENCETEMPLATE_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"

#include "portableserver_export.h"

class TAO_ObjectReferenceTemplate_Adapter;

/**
 * @class TAO_ObjectReferenceTemplate_Adapter_Factory
 *
 * @brief TAO_ObjectReferenceTemplate_Adapter_Factory.
 *
 * Class that creates one instance of TAO_ObjectReferenceTemplate_Adapter per
 * POA on the POA's first usage of its objectreferencetemplate_adapter.
 * This is a base class for the actual implementation in the
 * TAO_ObjectReferenceTemplate library.
 */
class TAO_PortableServer_Export TAO_ObjectReferenceTemplate_Adapter_Factory
  : public ACE_Service_Object
{
public:
  virtual ~TAO_ObjectReferenceTemplate_Adapter_Factory (void);

  virtual TAO_ObjectReferenceTemplate_Adapter * create (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  virtual void destroy (
      TAO_ObjectReferenceTemplate_Adapter * adapter
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
};

#include /**/ "ace/post.h"

#endif /* TAO_OBJECTREFERENCETEMPLATE_ADAPTER_FACTORY_H */
