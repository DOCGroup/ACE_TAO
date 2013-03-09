// -*- C++ -*-

// ===================================================================
/**
 *  @file ZIOP_Service_Context_Handler.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_ZIOP_SERVICE_CONTEXT_HANDLER_H
#define TAO_ZIOP_SERVICE_CONTEXT_HANDLER_H
#include /**/ "ace/pre.h"
#include "tao/ZIOP/ziop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Service_Context_Handler.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ZIOP_Service_Context_Handler :
  public TAO_Service_Context_Handler
{
public:
  virtual int process_service_context (TAO_Transport& transport,
                                       const IOP::ServiceContext& context,
                                       TAO_ServerRequest *request);
  virtual int generate_service_context (
    TAO_Stub *stub,
    TAO_Transport &transport,
    TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

#include /**/ "ace/post.h"
#endif /* TAO_ZIOP_SERVICE_CONTEXT_HANDLER_H */
