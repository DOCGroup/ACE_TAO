// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Dynamic_Adapter.h
//
// = AUTHOR
//     Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_DYNAMIC_ADAPTER_H
#define TAO_DYNAMIC_ADAPTER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_CORBA == 0)

#include "ace/Service_Object.h"

class TAO_GIOP_Twoway_Invocation;

class TAO_Export TAO_Dynamic_Adapter : public ACE_Service_Object
{
  // = TITLE
  //    TAO_Dynamic_Adapter.
  //
  // = DESCRIPTION
  //    Class that adapts various functions in the CORBA namespace
  //    related to DII/DSI, which is no longer found in the TAO library. 
  //    This is a base for the actual implementation in the DynamicInterface 
  //    library.
  //
public:
  virtual ~TAO_Dynamic_Adapter (void);

  // CORBA::Object::_create_request and CORBA::Object::_request.

  virtual void create_request (CORBA::Object_ptr obj,
                               CORBA::ORB_ptr orb,
                               const char *operation,
                               CORBA::NVList_ptr arg_list,
                               CORBA::NamedValue_ptr result,
                               CORBA::ExceptionList_ptr exceptions,
                               CORBA::Request_ptr &request,
                               CORBA::Flags req_flags,
                               CORBA_Environment &ACE_TRY_ENV =
                                 TAO_default_environment ());

  virtual CORBA::Request_ptr request (CORBA::Object_ptr obj,
                                      CORBA::ORB_ptr orb,
                                      const char *op,
                                      CORBA::Environment &ACE_TRY_ENV =
                                        TAO_default_environment ());

  // CORBA::is_nil and CORBA::release for Context, Request, and ServerRequest.

  virtual CORBA::Boolean context_is_nil (CORBA::Context_ptr ctx);

  virtual CORBA::Boolean request_is_nil (CORBA::Request_ptr req);

  virtual CORBA::Boolean server_request_is_nil (CORBA::ServerRequest_ptr req);

  virtual void context_release (CORBA::Context_ptr ctx);

  virtual void request_release (CORBA::Request_ptr req);

  virtual void server_request_release (CORBA::ServerRequest_ptr req);

  // CORBA::ORB::create_exception_list.

  virtual void create_exception_list (CORBA::ExceptionList_ptr &,
                                      CORBA_Environment &);

  // Decoding the user exception in the DII version of 
  // TAO_GIOP_Twoway_Invocation::invoke().

  virtual CORBA::Exception *decode_user_exception (
      CORBA::ExceptionList_ptr exceptions,
      TAO_GIOP_Twoway_Invocation *invocation,
      const char *buf,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
};

#endif /* TAO_HAS_MINIMUM_CORBA */
#include "ace/post.h"
#endif /* TAO_DYNAMIC_ADAPTER_H */
