// -*- C++ -*-

//=============================================================================
/**
 * @file LB_IORInterceptor.h
 *
 * $Id$

 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_LB_IOR_INTERCEPTOR_H
#define TAO_LB_IOR_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/// Forward declarations.
class TAO_LB_LoadAlert;


/**
 * @class TAO_LB_IORInterceptor
 *
 * @brief TAO Load Balancer IOR interceptor.
 *
 * This IORInterceptor is registered in object group member ORBs.
 * During creation of the object group member reference, this
 * IORInterceptor will change the "current_factory" attribute of the
 * PortableInterceptor::IORInfo object to the Load Balancer's
 * ObjectReferenceFactory.
 *
 * @see LB_ObjectReferenceFactory.h for details.
 */
class TAO_LB_IORInterceptor
  : public virtual PortableInterceptor::IORInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_LB_IORInterceptor (const CORBA::StringSeq & object_groups,
                         const CORBA::StringSeq & repository_ids,
                         const char * location,
                         CosLoadBalancing::LoadManager_ptr lm,
                         const char * orb_id,
                         TAO_LB_LoadAlert & load_alert);

  /**
   * @name Methods Required by the IOR Interceptor Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all IOR interceptors.
   */
  //@{
  /// Return the name of this IORInterceptor.
  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Cleanup resources acquired by this IORInterceptor.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the tagged components to the IOR.
  virtual void establish_components (
      PortableInterceptor::IORInfo_ptr info
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException
    )); 

  virtual void components_established (
      PortableInterceptor::IORInfo_ptr info
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void adapter_manager_state_changed (
      PortableInterceptor::AdapterManagerId id,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void adapter_state_changed (
      const PortableInterceptor::ObjectReferenceTemplateSeq & templates,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  //@}

private:

  /// Create and register the LoadAlert object with the LoadManager.
  void register_load_alert (ACE_ENV_SINGLE_ARG_DECL);

private:

  /// List of stringified object group references.
  const CORBA::StringSeq object_groups_;

  /// List of RepositoryIds for object that will be load
  /// managed/balanced.
  const CORBA::StringSeq repository_ids_;

  /// Location at which the LoadBalancing component resides.
  CORBA::String_var location_;

  /// Reference to the LoadManager.
  CosLoadBalancing::LoadManager_var lm_;

  /// ORBid of the ORB this IORInterceptor is registered with.
  CORBA::String_var orb_id_;

//   /// Reference to the LoadAlert servant.
//   TAO_LB_LoadAlert & load_alert_;

//   /// Reference to the LoadAlert object.
//   CosLoadBalancing::LoadAlert_var la_ref_;

//   /// Synchronize access to the class state.
//   TAO_SYNCH_MUTEX lock_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_LB_IOR_INTERCEPTOR_H */
