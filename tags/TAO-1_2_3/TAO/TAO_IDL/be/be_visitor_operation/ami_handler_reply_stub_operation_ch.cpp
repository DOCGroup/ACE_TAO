//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_reply_stub_operation_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for AMI Handler call back operation in
//    the client header.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_handler_reply_stub_operation_ch, "$Id$")


// ************************************************************
// Operation visitor for server header
// ************************************************************

be_visitor_operation_ami_handler_reply_stub_operation_ch::be_visitor_operation_ami_handler_reply_stub_operation_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_reply_stub_operation_ch::~be_visitor_operation_ami_handler_reply_stub_operation_ch (void)
{
}

int
be_visitor_operation_ami_handler_reply_stub_operation_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node


  // Generate the corresponding static skeleton method for this
  // operation only if there was no "native" type.
  if (!node->has_native ())
    {

      // Next line.
      *os << "static void ";

      // Check if we are an attribute node in disguise
      if (this->ctx_->attribute ())
        {
          // Now check if we are a "get" or "set" operation.
          if (node->nmembers () == 1)
            {
              *os << "_set_";
            }
          else
            {
              *os << "_get_";
            }
        }
      *os << node->local_name ()
          << "_reply_stub (" << be_idt << be_idt_nl;

      *os << "TAO_InputCDR &_tao_reply_cdr," << be_nl
          << "Messaging::ReplyHandler_ptr _tao_reply_handler," << be_nl
          << "CORBA::ULong reply_status";

      *os << be_nl
          << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl;
    }
  return 0;
}
