// $Id$

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID (be_visitor_operation, 
           be_visitor_operation_thru_poa_proxy_impl_ss, 
           "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_thru_poa_proxy_impl_ss::
be_visitor_operation_thru_poa_proxy_impl_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)

{
}

be_visitor_operation_thru_poa_proxy_impl_ss::
~be_visitor_operation_thru_poa_proxy_impl_ss (void)
{
  //No-Op.
}

int
be_visitor_operation_thru_poa_proxy_impl_ss::visit_operation (
    be_operation *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
          "visit_operation - "
          "bad interface scope\n"
        ),
        -1
      );
    }

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
          "visit_operation - "
          "Bad return type\n"
        ),
        -1
      );
    }

  // STEP 2: generate the return type mapping (same as in the header file)
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor_operation_rettype oro_visitor (&ctx);

  if (bt->accept (&oro_visitor) == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
          "visit_operation - "
          "codegen for return type failed\n"
        ),
        -1
      );
    }

  *os << " " << intf->full_thru_poa_proxy_impl_name () << "::";
  *os << node->local_name ();

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS);
  be_visitor_operation_arglist oapi_visitor (&ctx);

  if (node->accept (&oapi_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  *os << "{" << be_idt << be_nl;

  if (!be_global->exception_support ())
    {
      // Declare a return type
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS);
      be_visitor_operation_rettype_vardecl_ss ord_visitor (&ctx);

      if (bt->accept (&ord_visitor) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
              "visit_operation - "
              "codegen for return var decl failed\n"
            ),
            -1
          );
        }

      if (!this->void_return_type (bt))
        {
          os->indent ();
          *os << "ACE_UNUSED_ARG (_tao_retval);" << be_nl;
        }
    }

  //  os->indent ();

  *os <<"TAO_Object_Adapter::Servant_Upcall servant_upcall ("
      << be_idt << be_idt_nl
      << "_collocated_tao_target_->_stubobj ()"
      << "->servant_orb_var ()->orb_core ()"
      << be_uidt_nl
      << ");" << be_uidt_nl
      << "CORBA::Object_var forward_to;" << be_nl
      << "servant_upcall.prepare_for_upcall (" << be_idt << be_idt_nl
      << "_collocated_tao_target_->_object_key ()," << be_nl
      << "\"" << node->original_local_name () << "\"," << be_nl
      << "forward_to.out ()";

  if (!be_global->exception_support ())
    {
      *os << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_nl << be_uidt;
    }
  else
    {
      *os << be_uidt_nl << ");" << be_uidt_nl;
    }

  // Check if there is an exception.
  if (!be_global->exception_support ())
    if (this->gen_check_exception (bt) == -1)
      {
        ACE_ERROR_RETURN ((
            LM_ERROR,
            "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
            "visit_operation - "
            "codegen for checking exception failed\n"
          ),
          -1
        );
      }

  os->indent ();

  if (!this->void_return_type (bt))
    {
      *os << "return ";
    }

  *os << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << intf->full_skel_name () << "_ptr," << be_nl
      << "servant_upcall.servant ()->_downcast (" << be_idt << be_idt_nl
      << "\"" << intf->repoID ()  << "\"" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ")" << be_uidt;

  if (this->gen_invoke (ctx, node) == -1)
    {
      return -1;
    }

  if (this->void_return_type (bt))
    {
      *os << "return;\n";
    }

  *os << "}\n\n";

  os->decr_indent (0);

  return 0;
}


int
be_visitor_operation_thru_poa_proxy_impl_ss::gen_invoke (
    be_visitor_context &ctx,
    be_operation *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "->" << node->local_name () << " ("
      << be_idt << be_idt << be_idt_nl;

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS);
  be_visitor_operation_argument visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
          "gen_invoke - "
          "codegen for making upcall failed\n"
        ),
        -1
      );
    }

  // End the upcall
  *os << be_uidt_nl
      << ");";

  // Retrieve the operation return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (this->void_return_type (bt))
    {
      *os << be_uidt << be_uidt_nl;
    }
  else
    {
      *os << be_uidt << be_uidt << be_uidt_nl;
    }

  return 0;
}

