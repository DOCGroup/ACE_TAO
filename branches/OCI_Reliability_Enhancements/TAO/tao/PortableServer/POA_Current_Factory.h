// -*- C++ -*-

//=============================================================================
/**
 *  @file    POA_Current_Factory.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POA_CURRENT_FACTORY_H
#define TAO_POA_CURRENT_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_PortableServer_Export TAO_POA_Current_Factory
  : public TAO_Object_Loader
{
public:

  /// Creates a PICurrent and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);
};

ACE_STATIC_SVC_DECLARE (TAO_POA_Current_Factory)
ACE_FACTORY_DECLARE (TAO_PortableServer, TAO_POA_Current_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_ADAPTER_FACTORY_H */
