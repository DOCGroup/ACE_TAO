/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Servant_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth (fhunleth@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_SERVANT_DISPATCHER_H
#define TAO_SERVANT_DISPATCHER_H
#include "ace/pre.h"

#include "portableserver_export.h"
#include "Object_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Servant_Dispatcher
 *
 * @brief Interface for POA servant dispatching strategies.
 *
 */
class TAO_PortableServer_Export TAO_Servant_Dispatcher
{
public:
  virtual ~TAO_Servant_Dispatcher (void);

  /// Dispatch the request to the servant.
  virtual void dispatch (TAO_Object_Adapter::Servant_Upcall &servant_upcall,
                         TAO_ServerRequest &req,
                         CORBA::Environment &ACE_TRY_ENV) = 0;
};

#include "ace/post.h"
#endif /* TAO_SERVANT_DISPATCHER_H */
