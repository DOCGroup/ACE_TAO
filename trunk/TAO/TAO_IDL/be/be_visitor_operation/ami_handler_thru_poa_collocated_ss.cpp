//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_thru_poa_collocated_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for thru_poa_collocated Operation in the skeleton.
//
// = AUTHOR
//    Aniruddha Gokhale
//    Michael Kircher
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_handler_thru_poa_collocated_ss, "$Id$")


// *************************************************************************
//  be_visitor_operation_ami_handler_thru_poa_collocated_ss --
//  This visitor generates code for the thru_poa_collocated operation signature in a
//  server skeletons file
// *************************************************************************

be_visitor_operation_ami_handler_thru_poa_collocated_ss::be_visitor_operation_ami_handler_thru_poa_collocated_ss
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_ami_handler_thru_poa_collocated_ss::~be_visitor_operation_ami_handler_thru_poa_collocated_ss (void)
{
}

int be_visitor_operation_ami_handler_thru_poa_collocated_ss::visit_operation (be_operation *node)
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
                         "(%N:%l) be_visitor_operation_ami_handler_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }



  *os << "void " << intf->full_coll_name (be_interface::THRU_POA) << "::"
      << node->local_name () << " ";

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  // Generate the argument list with the appropriate mapping (same as
  // in the header file).
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if ((!visitor) || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;


  *os << "{" << be_idt_nl;

  // @@ Michael: Not sure about this one.
#if 0  
  if (!idl_global->exception_support ())
      *os << "ACE_DECLARE_NEW_CORBA_ENV;" << be_nl;
#endif /* 0 */

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
    {
      os->indent ();
      *os << "ACE_CHECK;\n";
    }

  os->indent ();
  *os << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << intf->full_skel_name () << "_ptr," << be_nl
      << "servant_upcall.servant ()->_downcast (" << be_idt << be_idt_nl
      << "\"" << intf->repoID ()  << "\"" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ")" << be_uidt;

  if (this->gen_invoke (ctx, node) == -1)
    return -1;

  os->indent ();
  *os << "return;";

  *os << be_uidt_nl
      << "}\n\n";

  return 0;
}

int be_visitor_operation_ami_handler_thru_poa_collocated_ss::gen_invoke (be_visitor_context &ctx,
                                                    be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "->";
    
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "set_";
      else
        *os << "get_";
    }

  *os << node->local_name () << " ("
      << be_idt << be_idt << "\n";

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!this->void_return_type (bt))
    {
      os->indent ();
      *os << "_tao_retval,\n";
    }

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_COLLOCATED_ARG_UPCALL_CS);
  if (this->has_param_type (node, AST_Argument::dir_INOUT)
      || this->has_param_type (node, AST_Argument::dir_OUT))
    ctx.sub_state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_HAS_ARGUMENTS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_thru_poa_collocated_ss::"
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
be_visitor_operation_ami_handler_thru_poa_collocated_ss::void_return_type (be_type *bt)
{
  // is the operation return type void?

  if (bt->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (bt)->pt ()
          == AST_PredefinedType::PT_void))
    return 1;
  else
    return 0;
}

int
be_visitor_operation_ami_handler_thru_poa_collocated_ss::has_param_type (be_operation *node,
                                      AST_Argument::Direction dir)
{
  // proceed if the number of members in our scope is greater than 0
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      0);
      // continue until each element is visited
      while (!si->is_done ())
	{
	  be_argument *bd = be_argument::narrow_from_decl (si->item ());
          if (bd && (bd->direction () == dir))
            return 1;

	  si->next ();
	} // end of while loop
      delete si;
    } // end of if

  // not of the type we are looking for
  return 0;
}
