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
                         TAO_ServerRequest &req,
                         CORBA::Environment &ACE_TRY_ENV);
};

#include "ace/post.h"
#endif /* TAO_DEFAULT_SERVANT_DISPATCHER_H */
