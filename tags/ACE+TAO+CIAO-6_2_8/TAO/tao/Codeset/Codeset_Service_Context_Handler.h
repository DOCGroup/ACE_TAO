// -*- C++ -*-

// ===================================================================
/**
 *  @file Codeset_Service_Context_Handler.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_CODESET_SERVICE_CONTEXT_HANDLER_H
#define TAO_CODESET_SERVICE_CONTEXT_HANDLER_H
#include /**/ "ace/pre.h"
#include "tao/Codeset/codeset_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Service_Context_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Stub;

class TAO_Codeset_Service_Context_Handler :
  public TAO_Service_Context_Handler
{
public:
  virtual int process_service_context (TAO_Transport& transport,
                                       const IOP::ServiceContext& context,
                                       TAO_ServerRequest *request);
  virtual int generate_service_context (
    TAO_Stub* stub,
    TAO_Transport &transport,
    TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_CODESET_SERVICE_CONTEXT_HANDLER_H */
