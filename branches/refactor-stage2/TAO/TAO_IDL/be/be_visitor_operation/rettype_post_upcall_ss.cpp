//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_post_upcall_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for post-processing of return type after an
//    upcall is made
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           rettype_post_upcall_ss, 
           "$Id$")

// ****************************************************************************
// visitor to do any post processing for return type after an upcall
// ****************************************************************************

be_visitor_operation_rettype_post_upcall_ss::
be_visitor_operation_rettype_post_upcall_ss (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = node;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }

  *os << bt->name () << "_forany _tao_retval_forany ("
      << be_idt << be_idt_nl
      << "_tao_retval.inout ()" << be_uidt_nl
      << ");\n" << be_uidt;

  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_post_upcall::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
