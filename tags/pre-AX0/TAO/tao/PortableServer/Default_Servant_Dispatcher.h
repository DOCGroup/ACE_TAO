/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Default_Servant_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth (fhunleth@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_DEFAULT_SERVANT_DISPATCHER_H
#define TAO_DEFAULT_SERVANT_DISPATCHER_H
#include "ace/pre.h"

#include "portableserver_export.h"
#include "Servant_Dispatcher.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Default_Servant_Dispatcher
 *
 * @brief Concrete instantiation of the Servant_Dispatcher strategy.
 *
 * This class just calls _dispatch on the servant_upcall.  No extra
 * processing is done or needed.
 *
 */
class TAO_PortableServer_Export TAO_Default_Servant_Dispatcher
  : public TAO_Servant_Dispatcher
{
public:
  virtual ~TAO_Default_Servant_Dispatcher (void);

  /// Dispatch the request to the servant.
  virtual void dispatch (TAO_Object_Adapter::Servant_Upcall &servant_upcall,
                         TAO_ServerRequest &req
                         ACE_ENV_ARG_DECL);

  /// Factory method for creating new POA's.
  virtual TAO_POA *create_POA (const ACE_CString &name,
                               TAO_POA_Manager &poa_manager,
                               const TAO_POA_Policy_Set &policies,
                               TAO_POA *parent,
                               ACE_Lock &lock,
                               TAO_SYNCH_MUTEX &thread_lock,
                               TAO_ORB_Core &orb_core,
                               TAO_Object_Adapter *object_adapter
                               ACE_ENV_ARG_DECL);
};

#include "ace/post.h"
#endif /* TAO_DEFAULT_SERVANT_DISPATCHER_H */
