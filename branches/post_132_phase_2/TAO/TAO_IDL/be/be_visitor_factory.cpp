// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL Backend
//
// = FILENAME
//    be_visitor_factory.cpp
//
// = DESCRIPTION
//    Defines a factory that returns a specialized visitor object based on the
//    code generation state. This factory creates visitors associated with
//    Asynchronous Messaging Invocations (AMI) since, because of implied IDL,
//    state is not locally known at visitor creation time.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "ast_argument.h"

#include "be_visitor_factory.h"
#include "be_visitor_context.h"

#include "be_visitor_decl.h"
#include "be_visitor_scope.h"
#include "be_visitor_interface/interface.h"
#include "be_visitor_interface/interface_ch.h"
#include "be_visitor_interface/ami_interface_ch.h"
#include "be_visitor_valuetype/valuetype.h"
#include "be_visitor_valuetype/valuetype_ch.h"
#include "be_visitor_valuetype/valuetype_obv_ch.h"
#include "be_visitor_valuetype/valuetype_obv_cs.h"
#include "be_visitor_eventtype/eventtype_obv_ch.h"
#include "be_visitor_eventtype/eventtype_obv_cs.h"
#include "be_visitor_valuetype/ami_exception_holder_ch.h"
#include "be_visitor_valuetype/ami_exception_holder_cs.h"
#include "be_visitor_operation/operation.h"
#include "be_visitor_operation/operation_ch.h"
#include "be_visitor_operation/operation_cs.h"
#include "be_visitor_operation/ami_ch.h"
#include "be_visitor_operation/ami_cs.h"
#include "be_visitor_operation/ami_handler_reply_stub_operation_ch.h"
#include "be_visitor_operation/ami_handler_reply_stub_operation_cs.h"
#include "be_visitor_operation/ami_exception_holder_operation_cs.h"

ACE_RCSID (be, 
           be_visitor_factory, 
           "$Id$")

TAO_Visitor_Factory::TAO_Visitor_Factory (void)
{
}

TAO_Visitor_Factory::~TAO_Visitor_Factory (void)
{
}

be_visitor *
TAO_Visitor_Factory::make_visitor (be_visitor_context *ctx)
{
  be_visitor *retval = 0;

  switch (ctx->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ACE_NEW_RETURN (retval,
                      be_visitor_interface_ch (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
      ACE_NEW_RETURN (retval,
                      be_visitor_valuetype_obv_ch (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
      ACE_NEW_RETURN (retval,
                      be_visitor_valuetype_obv_cs (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_EVENTTYPE_OBV_CH:
      ACE_NEW_RETURN (retval,
                      be_visitor_eventtype_obv_ch (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_EVENTTYPE_OBV_CS:
      ACE_NEW_RETURN (retval,
                      be_visitor_eventtype_obv_cs (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_OPERATION_CH:
      ACE_NEW_RETURN (retval,
                      be_visitor_operation_ch (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_OPERATION_CS:
      ACE_NEW_RETURN (retval,
                      be_visitor_operation_cs (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_AMI_INTERFACE_CH:
      ACE_NEW_RETURN (retval,
                      be_visitor_ami_interface_ch (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CH:
      ACE_NEW_RETURN (
          retval,
          be_visitor_operation_ami_handler_reply_stub_operation_ch (ctx),
          0
        );
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CS:
      ACE_NEW_RETURN (
          retval,
          be_visitor_operation_ami_handler_reply_stub_operation_cs (ctx),
          0
        );
      break;
    case TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CH:
      ACE_NEW_RETURN (retval,
                      be_visitor_operation_ami_ch (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CS:
      ACE_NEW_RETURN (retval,
                      be_visitor_operation_ami_cs (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_RAISE_OPERATION_CS:
      ACE_NEW_RETURN (
          retval,
          be_visitor_operation_ami_exception_holder_operation_cs (ctx),
          0
        );
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CH:
      ACE_NEW_RETURN (retval,
                      be_visitor_valuetype_ami_exception_holder_ch (ctx),
                      0);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CS:
      ACE_NEW_RETURN (retval,
                      be_visitor_valuetype_ami_exception_holder_cs (ctx),
                      0);
      break;
    default:
      // An error.
      return 0;
    }

  return retval;
}

