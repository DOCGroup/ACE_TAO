//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    thru_poa_collocated_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for thru_poa_collocated Operation in the skeleton.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, thru_poa_collocated_ss, "$Id$")


// *************************************************************************
//  be_visitor_operation_thru_poa_collocated_ss --
//  This visitor generates code for the thru_poa_collocated operation signature in a
//  server skeletons file
// *************************************************************************

be_visitor_operation_thru_poa_collocated_ss::be_visitor_operation_thru_poa_collocated_ss
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_thru_poa_collocated_ss::~be_visitor_operation_thru_poa_collocated_ss (void)
{
}

int be_visitor_operation_thru_poa_collocated_ss::visit_operation (be_operation *node)
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
                         "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
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
                         "be_visitor_operation_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "Bad visitor for return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  *os << " " << intf->full_coll_name (be_interface::THRU_POA) << "::";
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

  if (!idl_global->exception_support ())
    {
      // Declare a return type
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
                             "visit_operation - "
                             "codegen for return var decl failed\n"),
                            -1);
        }

      if (!this->void_return_type (bt))
        {
          os->indent ();
          *os << "ACE_UNUSED_ARG (";
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (bt->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
                                 "gen_check_exception - "
                                 "codegen failed\n"),
                                -1);
            }
          *os << ");\n";
        }
    }

  os->indent ();

  *os <<"TAO_Object_Adapter::Servant_Upcall servant_upcall ("
      << be_idt << be_idt_nl
      << "*this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()"
      << be_uidt_nl
      << ");" << be_uidt_nl
      << "servant_upcall.prepare_for_upcall (" << be_idt << be_idt_nl
      << "this->_object_key ()," << be_nl
      << "\"" << node->original_local_name () << "\"";
  if (!idl_global->exception_support ())
    *os << "," << be_nl
        << "ACE_TRY_ENV" << be_uidt_nl
        << ");\n" << be_uidt;
  else
    *os << be_uidt_nl << ");\n" << be_uidt;

  // check if there is an exception
  if (!idl_global->exception_support ())
    if (this->gen_check_exception (bt) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
                           "visit_operation - "
                           "codegen for checking exception failed\n"),
                          -1);
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
    return -1;

  if (this->void_return_type (bt))
    {
      os->indent ();
      *os << "return;";
    }

  *os << be_uidt_nl
      << "}\n\n";

  return 0;
}

int be_visitor_operation_thru_poa_collocated_ss::gen_invoke (be_visitor_context &ctx,
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
                         "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
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
be_visitor_operation_thru_poa_collocated_ss::gen_check_exception (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  // check if there is an exception
  if (!this->void_return_type (bt))
    {
      if (bt->size_type () == be_decl::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "ACE_CHECK_RETURN (0);\n";
        }
      else
        {
          *os << "ACE_CHECK_RETURN  (_tao_retval);\n";
        }
    }
  else
    {
      *os << "ACE_CHECK;\n";
    }

  return 0;
}

int
be_visitor_operation_thru_poa_collocated_ss::void_return_type (be_type *bt)
{
  // is the operation return type void?

  if (bt->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (bt)->pt ()
          == AST_PredefinedType::PT_void))
    return 1;
  else
    return 0;
}
