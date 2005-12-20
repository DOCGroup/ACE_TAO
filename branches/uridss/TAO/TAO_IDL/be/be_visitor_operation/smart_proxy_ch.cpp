//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_smart_proxy_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the client header.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           smart_proxy_ch, 
           "$Id$")

// ******************************************************
// Primary visitor for "operation" in client header.
// ******************************************************

be_visitor_operation_smart_proxy_ch::be_visitor_operation_smart_proxy_ch (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_smart_proxy_ch::~be_visitor_operation_smart_proxy_ch (
    void
  )
{
}

int
be_visitor_operation_smart_proxy_ch::visit_operation (be_operation *node)
{

  if (be_global->gen_smart_proxies ())
    {
      TAO_OutStream *os = this->ctx_->stream ();
      this->ctx_->node (node);
      os->indent ();

      *os << "virtual ";

      // STEP I: generate the return type.
      be_type *bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_smart_proxy_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      be_visitor_context ctx (*this->ctx_);
      be_visitor_operation_rettype or_visitor (&ctx);

      if (bt->accept (&or_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_smart_proxy_ch::"
                             "visit_operation - "
                             "codegen for return type failed\n"),
                            -1);
        }

      // STEP 2: generate the operation name.
      *os << " " << node->local_name ();

      // STEP 3: generate the argument list with the appropriate mapping. For these
      // we grab a visitor that generates the parameter listing
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_CH);
      be_visitor_operation_arglist oa_visitor (&ctx);

      if (node->accept (&oa_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_smart_proxy_ch::"
                             "visit_operation - "
                             "codegen for argument list failed\n"),
                            -1);
        }
    }
  else
    {
      ACE_UNUSED_ARG (node);
    }

  return 0;
}
