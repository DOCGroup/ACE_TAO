// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ORT_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_ORT_ADAPTER_FACTORY_H
#define TAO_ORT_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/SystemException.h"

#include "portableserver_export.h"

namespace TAO
{
  class ORT_Adapter;
}

namespace TAO
{
  /**
   * @class ORT_Adapter_Factory
   *
   * @brief ORT_Adapter_Factory
   *
   * Class that creates one instance of TAO::ORT_Adapter per
   * POA on the POA's first usage of its objectreferencetemplate_adapter.
   * This is a base class for the actual implementation in the
   * TAO_ObjectReferenceTemplate library.
   */
  class TAO_PortableServer_Export ORT_Adapter_Factory
    : public ACE_Service_Object
  {
  public:
    virtual ~ORT_Adapter_Factory (void);

    // @@ Johnny, is there need to specify an exception specification
    //    containing CORBA::SystemException for this method?  Removing
    //    it would allow us to avoid including "tao/SystemException.h"
    //    above.
    virtual TAO::ORT_Adapter * create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

    // @@ Johnny, same as above.
    virtual void destroy (
        TAO::ORT_Adapter * adapter
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ORT_ADAPTER_FACTORY_H */
