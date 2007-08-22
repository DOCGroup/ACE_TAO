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

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
class TAO_POAManager_Factory;

namespace PortableServer
{
  class POAManagerFactory;
  typedef POAManagerFactory *POAManagerFactory_ptr;
}
#endif

class TAO_PortableServer_Export TAO_POA_Manager :
  public PortableServer::POAManager,
  public TAO_Local_RefCounted_Object
{
  friend class TAO_Root_POA;
  friend class TAO_Object_Adapter;

public:

  void activate (void);

#if (TAO_HAS_MINIMUM_POA == 0)

  void hold_requests (CORBA::Boolean wait_for_completion);

  void discard_requests (CORBA::Boolean wait_for_completion);

  void deactivate (CORBA::Boolean etherealize_objects,
                   CORBA::Boolean wait_for_completion);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::POAManager::State get_state (void);

  char *get_id (void);

  TAO_POA_Manager (TAO_Object_Adapter &object_adapter,
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
                   const char * id,
                   const ::CORBA::PolicyList & policies,
                   PortableServer::POAManagerFactory_ptr poa_manager_factory);
#else
                   const char * id);
#endif

  ~TAO_POA_Manager (void);

  /// Check the state of this POA manager
  void check_state (void);

  PortableServer::POAManager::State get_state_i ();

  virtual CORBA::ORB_ptr _get_orb (void);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  CORBA::PolicyList& get_policies (void);
#endif

protected:

  void activate_i (void);

  void deactivate_i (CORBA::Boolean etherealize_objects,
                     CORBA::Boolean wait_for_completion);

  /// Method needed for notifying the IORInterceptors that the state
  /// of POAManager changed.
  void adapter_manager_state_changed (PortableServer::POAManager::State state);

#if (TAO_HAS_MINIMUM_POA == 0)

  void hold_requests_i (CORBA::Boolean wait_for_completion);

  void discard_requests_i (CORBA::Boolean wait_for_completion);

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

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

  TAO_POAManager_Factory& poa_manager_factory_;
  CORBA::PolicyList policies_;

#endif


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
# include "tao/PortableServer/POAManager.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_POAMANAGER_H */
