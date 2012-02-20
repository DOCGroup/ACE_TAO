
//=============================================================================
/**
 *  @file    tie_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for operations of the TIE class in the header
 *  file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "operation.h"

be_visitor_operation_tie_sh::be_visitor_operation_tie_sh (
    be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_tie_sh::~be_visitor_operation_tie_sh (void)
{
}

int
be_visitor_operation_tie_sh::visit_operation (be_operation *node)
{
  /// These implied IDL operations are not to be processed on
  /// the skeleton side.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // STEP I: generate the return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype oro_visitor (&ctx);

  if (bt->accept (&oro_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // STEP 2: generate the operation name.
  *os << " " << this->ctx_->port_prefix ().c_str ()
      << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  // We use this state here as a flag to leave out the param names in the
  // arglist, to avoid a possible clash with the TIE class template
  // parameter name.
  ctx.state (TAO_CodeGen::TAO_TIE_OPERATION_ARGLIST_SH);
  be_visitor_operation_arglist oa_visitor (&ctx);

  if (node->accept (&oa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  return 0;
}
