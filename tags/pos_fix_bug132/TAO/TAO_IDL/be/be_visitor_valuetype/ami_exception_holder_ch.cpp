//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_exception_holder_ch.cpp
//
// = DESCRIPTION
//    Visitor generating exception holders for the AMI callback model.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, ami_exception_holder_ch, "$Id$")

// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_valuetype_ami_exception_holder_ch::be_visitor_valuetype_ami_exception_holder_ch (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_ami_exception_holder_ch::~be_visitor_valuetype_ami_exception_holder_ch (void)
{
}

int
be_visitor_valuetype_ami_exception_holder_ch::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os; // output stream


  os = this->ctx_->stream ();

  // Open the namespace again.

  //*os << be_nl
  //    << "TAO_NAME_S

  // Generate the implemenation of the Messaging aware ORB
  *os << be_nl
      << "class _tao_" << node->local_name () << be_idt_nl
      << ": public ";

  if (!node->is_nested ())
    *os << "OBV_";

  *os << node->local_name () << "," << be_nl
      << "  public virtual OBV_Messaging::ExceptionHolder," << be_nl
      << "  public virtual CORBA::DefaultValueRefCountBase" << be_uidt_nl
      << "{" << be_idt_nl;

  *os << be_uidt_nl << "public:" << be_idt_nl;

  *os << "_tao_" << node->local_name () << " ();" << be_nl << be_nl;

  *os << "~_tao_" << node->local_name () << " ();\n\n";

  if (this->visit_valuetype_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ami_exception_holder_cs::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"), -1);
    }

  os->indent ();

  *os << be_uidt_nl
      << "};" << be_nl << be_nl;

  // Create code for the valuetype factory

  *os << "class " << node->local_name () << "_factory : public "
      << node->full_name () << "_init" << be_idt_nl
      << "{" << be_idt_nl
      << "friend class " << node->local_name () << ";" << be_nl
      << be_uidt_nl << "public:" << be_idt_nl
      << "// create (...) would go here" << be_nl
      << be_uidt_nl << "private:" << be_idt_nl;

  *os << "virtual ~" << node->local_name () << "_factory ();" << be_nl << be_nl;

  *os << "TAO_OBV_CREATE_RETURN_TYPE (" << node->local_name ()
      << ")  create_for_unmarshal ();" << be_nl << be_uidt_nl
      << "};" << be_uidt_nl;

  // close the namespace again.
  //*os << "}" << be_nl
  //    << "TAO_NAMESPACE_CLOSE" << be_nl << be_nl;

  return 0;
}

int
be_visitor_valuetype_ami_exception_holder_ch::visit_operation (be_operation *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ami_exception_holder_ch::"
                         "visit_operation - "
                         "Bad visitor to argument list\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ami_exception_holder_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  return 0;
}
