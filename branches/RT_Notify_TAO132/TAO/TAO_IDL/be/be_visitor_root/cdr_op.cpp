//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op.cpp
//
// = DESCRIPTION
//    Visitor generating code for the CDR operators for types defined
//    in Root's scope.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_root, 
           cdr_op, 
           "$Id$")

// ***************************************************************************
// Root visitor for generating CDR operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_root_cdr_op::be_visitor_root_cdr_op (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cdr_op::~be_visitor_root_cdr_op (void)
{
}

int
be_visitor_root_cdr_op::visit_root (be_root *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CDR_OP_CH)
    {
      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      *os << "\n\n#ifndef __ACE_INLINE__";
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_cdr_op::visit_root - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CDR_OP_CH)
    {
      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      *os << "\n\n#endif /* __ACE_INLINE__ */";
    }

  return 0;
}
