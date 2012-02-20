
//=============================================================================
/**
 *  @file    smart_proxy_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the stubs file.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#include "operation.h"

be_visitor_operation_smart_proxy_cs::be_visitor_operation_smart_proxy_cs (
    be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_smart_proxy_cs::~be_visitor_operation_smart_proxy_cs (
    void
  )
{
}

int be_visitor_operation_smart_proxy_cs::visit_operation (be_operation *node)
{
  if (be_global->gen_smart_proxies ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      // We need the interface node in which this operation was defined. However,
      // if this operation node was an attribute node in disguise, we get this
      // information from the context.
      UTL_Scope *s =
        this->ctx_->attribute ()
          ? this->ctx_->attribute ()->defined_in ()
          : node->defined_in ();

      be_interface *intf = be_interface::narrow_from_scope (s);

      if (intf == 0)
        {
          be_porttype *pt = be_porttype::narrow_from_scope (s);

          if (pt == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_operation_")
                                 ACE_TEXT ("smart_proxy_cs::")
                                 ACE_TEXT ("visit_operation - ")
                                 ACE_TEXT ("bad scope\n")),
                                -1);
            }
          else
            {
              intf = this->ctx_->interface ();
            }
        }

      be_type *bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_smart_proxy_cs::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // STEP 2: generate the return type mapping (same as in the header file).
      be_visitor_context ctx (*this->ctx_);
      be_visitor_operation_rettype oro_visitor (&ctx);

      if (bt->accept (&oro_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_smart_proxy_cs::"
                             "visit_operation - "
                             "codegen for return type failed\n"),
                            -1);
        }

      // Its necessary to take care of the nested case. The smart proxy classes
      // are in the same scope as the proxy (i.e. interface) but here the
      // node (i.e the operation) is already in a scope lower than intf
      // (i.e. be_interface), so for deciding the exact scope use intf.

      be_decl* scope = be_scope::narrow_from_scope (intf->defined_in ())->decl ();

      *os << " ";
      *os << scope->full_name ();

      // Only if there exists any nesting "::" is needed!
      if (intf->is_nested ())
        *os << "::";

      *os << "TAO_" << intf->flat_name () <<"_Smart_Proxy_Base::";
      *os << this->ctx_->port_prefix ().c_str ()
          << node->local_name () << " ";

      // STEP 4: generate the argument list with the appropriate mapping (same as
      // in the header file)
      ctx = *this->ctx_;
      be_visitor_operation_arglist oao_visitor (&ctx);

      if (node->accept (&oao_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ss::"
                             "visit_operation - "
                             "codegen for argument list failed\n"),
                            -1);
        }

      *os << "{" << be_idt << "\n";

      os->indent ();

      // Simply make the call and leave everything to the layer
      // above, i.e. the client code.
      if (!this->void_return_type (bt))
        {
          *os << "return ";
        }

      *os << "this->get_proxy ()";

      if (this->gen_invoke (ctx, node) == -1)
        return -1;

      *os << be_uidt_nl
          << "}\n\n";
    }
  else
    {
      ACE_UNUSED_ARG (node);
    }

  return 0;
}

int be_visitor_operation_smart_proxy_cs::gen_invoke (be_visitor_context &ctx,
                                                     be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "->" << node->local_name () << " ("
      << be_idt << be_idt << "\n";

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS);
  be_visitor_operation_argument visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_smart_proxy_cs::"
                         "gen_invoke - "
                         "codegen for making upcall failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << ");\n" << be_uidt;

  return 0;
}

