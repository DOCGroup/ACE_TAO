// -*- C++ -*-

// ===================================================================
/**
 *  @file   Service_Context_Handler.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_SERVICE_CONTEXT_HANDLER_H
#define TAO_SERVICE_CONTEXT_HANDLER_H

#include /**/ "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;
class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_Stub;
class TAO_ServerRequest;

/**
 * This is the base of handler classes that each can process a certain
 * service context. This way optional libraries can plugin handlers to the
 * core of TAO. Each handler has to be registered in the Service Context
 * Handler registry
 */
class TAO_Export TAO_Service_Context_Handler
{
  public:
    TAO_Service_Context_Handler (void);
    virtual int process_service_context (
      TAO_Transport& transport,
      const IOP::ServiceContext& context,
      TAO_ServerRequest *request) = 0;

    virtual int generate_service_context (
      TAO_Stub *stub,
      TAO_Transport& transport,
      TAO_Operation_Details &opdetails,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg) = 0;

    virtual ~TAO_Service_Context_Handler (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SERVICE_CONTEXT_HANDLER_H */
