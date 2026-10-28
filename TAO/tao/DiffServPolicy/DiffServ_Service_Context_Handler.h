// -*- C++ -*-

// ===================================================================
/**
 *  @file DiffServ_Service_Context_Handler.h
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_DIFFSERV_SERVICE_CONTEXT_HANDLER_H
#define TAO_DIFFSERV_SERVICE_CONTEXT_HANDLER_H
#include /**/ "ace/pre.h"
#include "tao/DiffServPolicy/DiffServPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Service_Context_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DiffServ_Service_Context_Handler :
  public TAO_Service_Context_Handler
{
public:
  TAO_DiffServ_Service_Context_Handler () = default;

  int process_service_context (TAO_Transport& transport,
                                       const IOP::ServiceContext& context,
                                       TAO_ServerRequest *request) override;
  int generate_service_context (
    TAO_Stub *stub,
    TAO_Transport &transport,
    TAO_Operation_Details &opdetails,
    TAO_Target_Specification &spec,
    TAO_OutputCDR &msg) override;

private:
  TAO_DiffServ_Service_Context_Handler (TAO_DiffServ_Service_Context_Handler const &) = delete;
  TAO_DiffServ_Service_Context_Handler &operator= (
    TAO_DiffServ_Service_Context_Handler const &) = delete;
  TAO_DiffServ_Service_Context_Handler (TAO_DiffServ_Service_Context_Handler &&) = delete;
  TAO_DiffServ_Service_Context_Handler &operator= (
    TAO_DiffServ_Service_Context_Handler &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DIFFSERV_SERVICE_CONTEXT_HANDLER_H */
