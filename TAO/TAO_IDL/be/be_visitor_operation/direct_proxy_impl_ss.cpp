// $Id$

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID (be_visitor_operation, 
           direct_proxy_impl_ss, 
           "$Id$")

be_visitor_operation_direct_proxy_impl_ss::
be_visitor_operation_direct_proxy_impl_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_direct_proxy_impl_ss::
~be_visitor_operation_direct_proxy_impl_ss (void)
{
}

int
be_visitor_operation_direct_proxy_impl_ss::visit_operation (
    be_operation *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context.
  be_interface *intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_direct_collocated_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_direct_collocated_ss::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  os->indent ();
  // STEP 2: generate the return type mapping (same as in the header file)
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor_operation_rettype oro_visitor (&ctx);

  if (bt->accept (&oro_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_direct_collocated_ss::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  *os << " " << intf->full_direct_proxy_impl_name () << "::"
      << node->local_name () << " ";

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS);
  be_visitor_operation_arglist oapi_visitor (&ctx);

  if (node->accept (&oapi_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  *os << "{" << be_idt << "\n";

  os->indent ();

  if (!this->void_return_type (bt))
    {
      *os << "return ";
    }

  *os << "ACE_reinterpret_cast ("
      << be_idt << be_idt_nl //idt = 3
      << intf->full_skel_name () << "_ptr,"
      << be_nl
      <<"_collocated_tao_target_->_servant ()->_downcast ("
      << "\"" << intf->repoID () << "\"" << ")"
      << be_uidt_nl // idt = 2
      << ")";

  if (this->gen_invoke (ctx, node) == -1)
    {
      return -1;
    }

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;

  return 0;
}



int
be_visitor_operation_direct_proxy_impl_ss::gen_invoke (
    be_visitor_context &ctx,
    be_operation *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "->" << node->local_name () << " ("
      << be_idt << be_idt << be_idt_nl ;

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS);
  be_visitor_operation_argument visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_direct_collocated_ss::"
                         "gen_invoke - "
                         "codegen for making upcall failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << ");" << be_uidt << be_uidt <<  be_uidt_nl;

  return 0;
}

