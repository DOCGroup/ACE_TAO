// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request_Dispatcher.h
 *
 *  $Id$
 *
 *  A class that strategizes the request dispatching procedure.
 *
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_REQUEST_DISPATCHER_H
#define TAO_REQUEST_DISPATCHER_H
#include "ace/pre.h"

#include "tao/Object_KeyC.h"
#include "tao/IOPC.h"


class TAO_ServerRequest;

/**
 * @class TAO_Request_Dispatcher
 *
 * @brief A class that strategizes the request dispatching procedure.
 *
 */
class TAO_Export TAO_Request_Dispatcher
{
public:
  /// Destructor.
  virtual ~TAO_Request_Dispatcher (void);

  /**
   * Dispatch a request.
   */
  virtual void dispatch (TAO_ORB_Core *orb_core,
                         TAO_ServerRequest &request,
                         CORBA::Object_out forward_to
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#include "ace/post.h"
#endif /* TAO_REQUEST_DISPATCHER_H */
