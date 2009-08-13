// -*- C++ -*-


//=============================================================================
/**
 *  @file    Dynamic_Adapter_Impl.h
 *
 *  $Id$
 *
 *  Header file for class TAO_Dynamic_Adapter_Impl.
 *
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNAMIC_ADAPTER_IMPL_H
#define TAO_DYNAMIC_ADAPTER_IMPL_H
#include /**/ "ace/pre.h"

#include "tao/Dynamic_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicInterface/dynamicinterface_export.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Dynamic_Adapter_Impl
 *
 * Concrete subclass of TAO_Dynamic_Adapter
 * in the TAO library. This class helps implement various
 * functions in the CORBA namespace relating to DII/DSI
 * invocations.
 */
class TAO_DynamicInterface_Export TAO_Dynamic_Adapter_Impl
  : public TAO_Dynamic_Adapter
{
public:
  TAO_Dynamic_Adapter_Impl (void);
  virtual ~TAO_Dynamic_Adapter_Impl (void);

  // CORBA::Object::_create_request and CORBA::Object::_request.

  virtual void create_request (CORBA::Object_ptr obj,
                               CORBA::ORB_ptr orb,
                               const char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::ExceptionList_ptr exceptions,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags);

  virtual CORBA::Request_ptr request (CORBA::Object_ptr obj,
                                      CORBA::ORB_ptr orb,
                                      const char *op);

  // CORBA::is_nil and CORBA::release for Context, Request, and ServerRequest.

  virtual CORBA::Boolean context_is_nil (CORBA::Context_ptr ctx);

  virtual CORBA::Boolean request_is_nil (CORBA::Request_ptr req);

  virtual CORBA::Boolean server_request_is_nil (CORBA::ServerRequest_ptr req);

  virtual void context_release (CORBA::Context_ptr ctx);

  virtual void request_release (CORBA::Request_ptr req);

  virtual void server_request_release (CORBA::ServerRequest_ptr req);

  // CORBA::ORB::create_exception_list.

  virtual void create_exception_list (CORBA::ExceptionList_ptr &list);

  // Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_Request_Factory_Initializer =
  TAO_Dynamic_Adapter_Impl::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Dynamic_Adapter_Impl)
ACE_FACTORY_DECLARE (TAO_DynamicInterface, TAO_Dynamic_Adapter_Impl)

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMIC_ADAPTER_IMPL_H */
