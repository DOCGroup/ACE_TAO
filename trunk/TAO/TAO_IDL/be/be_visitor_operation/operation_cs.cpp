//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale & Angelo Corsaro
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_cs::be_visitor_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
    //    operation_name_ (0)
{
}

be_visitor_operation_cs::~be_visitor_operation_cs (void)
{
  //  delete[] operation_name_;
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_cs::visit_operation (be_operation *node)
{
  be_interface *intf = this->ctx_->attribute ()
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

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  if (node->is_local ())
    {
      return 0;
    }

  os->indent (); // start with the current indentation level

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Generate the return type mapping (same as in the header file)
  be_visitor_context ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if ((!visitor) || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  delete visitor;

  // Generate the operation name
  *os << " " << node->name ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);

  if ((!visitor) || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  *os << "{" << be_idt_nl;
  *os << this->gen_environment_var ();

  if (be_global->exception_support ())
    {
      *os << be_nl << be_nl;
    }

  // For what follows, the return type node nust be unaliased.
  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      be_typedef *btd = be_typedef::narrow_from_decl (bt);
      bt = btd->primitive_base_type ();
    }

  AST_Decl::NodeType bnt = bt->base_node_type ();
  be_predefined_type *bpt = 0;
  AST_PredefinedType::PredefinedType pdt = AST_PredefinedType::PT_void;

  if (bnt == AST_Decl::NT_pre_defined)
    {
      bpt = be_predefined_type::narrow_from_decl (bt);
      pdt = bpt->pt ();

      if (pdt == AST_PredefinedType::PT_longlong)
        {
          *os << "CORBA::LongLong _tao_retval = "
              << "ACE_CDR_LONGLONG_INITIALIZER;" << be_nl << be_nl;
        }
      else if (pdt == AST_PredefinedType::PT_longdouble)
        {
          *os << "CORBA::LongDouble _tao_retval = "
              << "ACE_CDR_LONG_DOUBLE_INITIALIZER;" << be_nl << be_nl;
        }
    }

  // Generate code that retrieves the proper proxy implementation
  // using the proxy broker available, and perform the call
  // using the proxy implementation provided by the broker.

  *os << intf->base_proxy_impl_name () << " &proxy = " << be_idt_nl
      << "this->the" << intf->base_proxy_broker_name ()
      << "_->select_proxy (this, ACE_TRY_ENV);" << be_uidt_nl;

  if (!this->void_return_type (bt))
    {
      *os << "ACE_CHECK_RETURN (";

      if (bnt == AST_Decl::NT_enum)
        {
          // The enum is a unique type, so we must cast.
          *os << "(" << bt->name () << ")0);";
        }
      else if (bnt == AST_Decl::NT_struct || bnt == AST_Decl::NT_union)
        {
          be_decl *bd = be_decl::narrow_from_decl (bt);

          if (bd->size_type () == be_decl::FIXED)
            {
              // For a fixed size struct or union the return value
              // is not a pointer, so we call the default constructor
              // and return the result.
              *os << bt->name () << " ());";
            }
          else
            {
              *os << "0);";
            }
        }
      else if (bnt == AST_Decl::NT_pre_defined)
        {
          if (pdt == AST_PredefinedType::PT_longlong
              || pdt == AST_PredefinedType::PT_longdouble)
            {
              *os << "_tao_retval);";
            }
          else
            {
              *os << "0);";
            }
        }
      else
        {
          *os << "0);";
        }
      
      *os << be_nl << be_nl
          << "return ";
    }
  else
    {
      *os << "ACE_CHECK;" << be_nl << be_nl;
    }

  *os << "proxy." << node->local_name ()
      << " (" << be_idt << be_idt_nl << "this";

  if (node->nmembers () > 0)
    {

      // Initialize an iterator to iterate over our scope.
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);

      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();

          if (d == 0)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"),
                                -1);

            }

	        *os << "," << be_nl;
          be_decl *decl = be_decl::narrow_from_decl (d);

	        *os << decl->local_name();
	        si->next ();
	      }
    }

  if (!be_global->exception_support ())
    {
      *os << "," << be_nl << "ACE_TRY_ENV";
    }

  *os << be_uidt_nl << ");" << be_uidt << be_uidt_nl << "}\n\n";

  return 0;
}

int
be_visitor_operation_cs::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";
  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "PARAM_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "PARAM_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "PARAM_OUT, ";
      break;
    }
  *os << "0}";

  return 0;
}
