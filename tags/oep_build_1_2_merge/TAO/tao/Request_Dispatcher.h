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

#include /**/ "ace/pre.h"

#include "tao/Exception.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Objref_VarOut_T.h"

class TAO_ORB_Core;
class TAO_ServerRequest;

namespace CORBA
{
  class Object;
  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object, Object_var> Object_out;
}

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

#include /**/ "ace/post.h"

#endif /* TAO_REQUEST_DISPATCHER_H */
