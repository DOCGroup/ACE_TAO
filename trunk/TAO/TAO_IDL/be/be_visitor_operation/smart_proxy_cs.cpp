//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_smart_proxy_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_smart_proxy_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_smart_proxy_cs::be_visitor_operation_smart_proxy_cs
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_smart_proxy_cs::~be_visitor_operation_smart_proxy_cs (void)
{
}

int be_visitor_operation_smart_proxy_cs::visit_operation (be_operation *node)
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_smart_proxy_cs::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_smart_proxy_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // STEP 2: generate the return type mapping (same as in the header file)
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_smart_proxy_cs::"
                         "visit_operation - "
                         "Bad visitor for return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_smart_proxy_cs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

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
  *os << node->local_name () << " ";

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ss::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

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
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_smart_proxy_cs::"
                         "gen_invoke - "
                         "codegen for making upcall failed\n"),
                        -1);
    }

  // end the upcall
  *os << be_uidt_nl
      << ");\n" << be_uidt;
  return 0;
}

int
be_visitor_operation_smart_proxy_cs::void_return_type (be_type *bt)
{
  // is the operation return type void?

  if (bt->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (bt)->pt ()
          == AST_PredefinedType::PT_void))
    return 1;
  else
    return 0;
}
