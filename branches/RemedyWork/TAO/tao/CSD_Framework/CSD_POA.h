// -*- C++ -*-

//=============================================================================
/**
 *  @file CSD_POA.h
 *
 *  $Id$
 *
 *  @author Yan Dai (dai_y@ociweb.com)
 */
//=============================================================================

#ifndef TAO_CSD_POA_H
#define TAO_CSD_POA_H

#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Regular_POA.h"
#include "tao/CSD_Framework/CSD_Strategy_Proxy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CSD_POA
 *
 * @brief Implementation of the CSD_Framework::POA interface.
 *
 * Implementation of the CSD_Framework::POA interface.
 */
class TAO_CSD_FW_Export TAO_CSD_POA
  : public virtual CSD_Framework::POA,
    public virtual TAO_Regular_POA
{
public:
  /// Constructor
  TAO_CSD_POA (const String &name,
               PortableServer::POAManager_ptr poa_manager,
               const TAO_POA_Policy_Set &policies,
               TAO_Root_POA *parent,
               ACE_Lock &lock,
               TAO_SYNCH_MUTEX &thread_lock,
               TAO_ORB_Core &orb_core,
               TAO_Object_Adapter *object_adapter);

  /// Destructor
  virtual ~TAO_CSD_POA (void);

  /// Pass the Strategy object reference to the CSD poa.
  virtual
  void set_csd_strategy (::CSD_Framework::Strategy_ptr s);

  /// Hook - The POA has been (or is being) activated.
  virtual void poa_activated_hook ();

  /// Hook - The POA has been deactivated.
  virtual void poa_deactivated_hook ();

  /// Hook - A servant has been activated.
  virtual void servant_activated_hook (PortableServer::Servant servant,
                              const PortableServer::ObjectId& oid);

  /// Hook - A servant has been deactivated.
  virtual void servant_deactivated_hook (PortableServer::Servant servant,
                                const PortableServer::ObjectId& oid);

  /// Method for creating new CSD POA.
  TAO_Root_POA * new_POA (const String &name,
                          PortableServer::POAManager_ptr poa_manager,
                          const TAO_POA_Policy_Set &policies,
                          TAO_Root_POA *parent,
                          ACE_Lock &lock,
                          TAO_SYNCH_MUTEX &thread_lock,
                          TAO_ORB_Core &orb_core,
                          TAO_Object_Adapter *object_adapter);

  /// Servant Dispatching Strategy proxy accessor.
  TAO::CSD::Strategy_Proxy&
    servant_dispatching_strategy_proxy (void) const;

private:

  TAO::CSD::Strategy_Proxy* sds_proxy_;
};

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined (__ACE_INLINE__)
# include "tao/CSD_Framework/CSD_POA.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_POA_H  */

