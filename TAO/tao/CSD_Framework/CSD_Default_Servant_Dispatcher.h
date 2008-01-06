// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_Default_Servant_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Yan Dai (dai_y@ociweb.com)
 */
//=============================================================================

#ifndef TAO_CSD_DEFAULT_SERVANT_DISPATCHER_H
#define TAO_CSD_DEFAULT_SERVANT_DISPATCHER_H
#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Default_Servant_Dispatcher.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CSD_Default_Servant_Dispatcher
 *
 * @brief Interface for CSD_POA servant dispatching strategies.
 */
class TAO_CSD_FW_Export TAO_CSD_Default_Servant_Dispatcher
  : public virtual TAO_Default_Servant_Dispatcher
{
public:
  virtual ~TAO_CSD_Default_Servant_Dispatcher (void);

  /// Factory method for creating new CSD Root POA.
  virtual TAO_Root_POA *create_Root_POA (const ACE_CString &name,
                                         PortableServer::POAManager_ptr poa_manager,
                                         const TAO_POA_Policy_Set &policies,
                                         ACE_Lock &lock,
                                         TAO_SYNCH_MUTEX &thread_lock,
                                         TAO_ORB_Core &orb_core,
                                         TAO_Object_Adapter *object_adapter);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_CSD_DEFAULT_SERVANT_DISPATCHER_H */
