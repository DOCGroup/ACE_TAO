// -*- C++ -*-

//=============================================================================
/**
 *  @file    POAManager.h
 *
 *  $Id$
 *
 *   POAManager
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_POAMANAGER_H
#define TAO_POAMANAGER_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/poa_macros.h"
#include "tao/PortableServer/POAManagerC.h"

// Local Object
#include "tao/LocalObject.h"
#include "tao/PI_ForwardC.h"
#include "ace/Unbounded_Set.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decl.
class TAO_Root_POA;
class TAO_Object_Adapter;
class TAO_POAManager_Factory;

namespace PortableServer
{
  class POAManagerFactory;
  typedef POAManagerFactory *POAManagerFactory_ptr;
}

class TAO_PortableServer_Export TAO_POA_Manager :
  public PortableServer::POAManager,
  public TAO_Local_RefCounted_Object
{
  friend class TAO_Root_POA;
  friend class TAO_Object_Adapter;

public:

  void activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

#if (TAO_HAS_MINIMUM_POA == 0)

  void hold_requests (CORBA::Boolean wait_for_completion
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

  void discard_requests (CORBA::Boolean wait_for_completion
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

  void deactivate (CORBA::Boolean etherealize_objects,
                   CORBA::Boolean wait_for_completion
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::POAManager::State get_state (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  char *get_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  TAO_POA_Manager (TAO_Object_Adapter &object_adapter,
                   const char * id,
                   const ::CORBA::PolicyList & policies,
                   PortableServer::POAManagerFactory_ptr poa_manager_factory);

  ~TAO_POA_Manager (void);

  /// Check the state of this POA manager
  void check_state (ACE_ENV_SINGLE_ARG_DECL);

  PortableServer::POAManager::State get_state_i ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ORB_ptr _get_orb (
      ACE_ENV_SINGLE_ARG_DECL
    );

  CORBA::PolicyList& get_policies ();

protected:

  void activate_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

  void deactivate_i (CORBA::Boolean etherealize_objects,
                     CORBA::Boolean wait_for_completion
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

  /// Method needed for notifying the IORInterceptors that the state
  /// of POAManager changed.
  void adapter_manager_state_changed (PortableServer::POAManager::State state
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if (TAO_HAS_MINIMUM_POA == 0)

  void hold_requests_i (CORBA::Boolean wait_for_completion
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

  void discard_requests_i (CORBA::Boolean wait_for_completion
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::POAManager::AdapterInactive));

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_Lock &lock (void);

  int remove_poa (TAO_Root_POA *poa);

  int register_poa (TAO_Root_POA *poa);

protected:

  PortableServer::POAManager::State state_;

  ACE_Lock &lock_;

  typedef ACE_Unbounded_Set<TAO_Root_POA *> POA_COLLECTION;

  POA_COLLECTION poa_collection_;

  TAO_Object_Adapter &object_adapter_;

  CORBA::String_var id_;

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  TAO_POAManager_Factory& poa_manager_factory_;
#endif

  CORBA::PolicyList policies_;

private :

  /**
   * Generate an id for this POAManager.
   * @return A value that uniquely identifies the POAManager within a
   *         given process.
   * @note: The id_ has the ownership of the memory allocated in this method.
   */
  char* generate_manager_id (void) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/POAManager.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_POAMANAGER_H */
