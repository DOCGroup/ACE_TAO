// -*- C++ -*-

//=============================================================================
/**
 *  @file    POAManagerFactory.h
 *
 *  $Id$
 *
 *   POAManagerFactory
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POAMANAGERFACTORY_H
#define TAO_POAMANAGERFACTORY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/POAManagerFactoryC.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/LocalObject.h"
#include "ace/Unbounded_Set.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_PortableServer_Export TAO_POAManager_Factory :
  public ::PortableServer::POAManagerFactory,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_POAManager_Factory (TAO_Object_Adapter &object_adapter);

  virtual ~TAO_POAManager_Factory (void);

  virtual ::PortableServer::POAManager_ptr create_POAManager (
      const char * id,
      const ::CORBA::PolicyList & policies);

  virtual ::PortableServer::POAManagerFactory::POAManagerSeq * list (void);

  virtual ::PortableServer::POAManager_ptr find (const char * id);

  void remove_all_poamanagers (void);

  int remove_poamanager (::PortableServer::POAManager_ptr poamanager);

  int register_poamanager (::PortableServer::POAManager_ptr poamanager);

private:
  TAO_Object_Adapter &object_adapter_;

  typedef ACE_Unbounded_Set < ::PortableServer::POAManager_ptr> POAMANAGERSET;

  POAMANAGERSET poamanager_set_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POAMANAGERFACTORY_H */
