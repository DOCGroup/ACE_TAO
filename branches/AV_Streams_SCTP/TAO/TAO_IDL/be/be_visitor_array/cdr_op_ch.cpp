//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Arrays for the Cdr operators in the client
//    header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_visitor_sequence/cdr_op_ch.h"

ACE_RCSID (be_visitor_array, 
           cdr_op_ch, 
           "$Id$")

// ***************************************************************************
// Array visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_array_cdr_op_ch::be_visitor_array_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_array_cdr_op_ch::~be_visitor_array_cdr_op_ch (void)
{
}

int
be_visitor_array_cdr_op_ch::visit_array (be_array *node)
{
  if (node->cli_hdr_cdr_op_gen () || node->imported () || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  AST_Decl::NodeType nt = bt->node_type ();

  // If the node is an array of anonymous sequence, we need to
  // generate the sequence's cdr operator declaration here.
  if (nt == AST_Decl::NT_sequence && bt->anonymous ())
    {
      be_visitor_sequence_cdr_op_ch visitor (this->ctx_);

      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_cdr_op_ch::"
                             "visit_array - "
                             "accept on anonymous base type failed\n"),
                            -1);
        }
    }

  // If the array is an anonymous member and if its element type
  // is a declaration (not a reference), we must generate code for
  // the declaration.
  if (this->ctx_->alias () == 0 // Not a typedef.
      && bt->is_child (this->ctx_->scope ()))
    {
      int status = 0;
      be_visitor_context ctx (*this->ctx_);

      switch (nt)
      {
        case AST_Decl::NT_enum:
          {
            be_visitor_enum_cdr_op_ch ec_visitor (&ctx);
            status = bt->accept (&ec_visitor);
            break;
          }
        case AST_Decl::NT_struct:
          {
            be_visitor_structure_cdr_op_ch sc_visitor (&ctx);
            status = bt->accept (&sc_visitor);
            break;
          }
        case AST_Decl::NT_union:
          {
            be_visitor_union_cdr_op_ch uc_visitor (&ctx);
            status = bt->accept (&uc_visitor);
            break;
          }
        default:
          break;
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "array base type codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the CDR << and >> operator declarations.
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator<< (TAO_OutputCDR &, const ";

  if (!this->ctx_->tdef ())
    {
      be_scope* scope = be_scope::narrow_from_scope (node->defined_in ());
      be_decl* parent = scope->decl ();

      *os << parent->full_name ()
          << "::_" << node->local_name ()
          << "_forany &);" << be_nl;
    }
  else
    {
      *os << node->name () << "_forany &);" << be_nl;
    }

  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>> (TAO_InputCDR &, ";

  if (!this->ctx_->tdef ())
    {
      be_scope* scope = be_scope::narrow_from_scope (node->defined_in ());
      be_decl* parent = scope->decl ();

      *os << parent->full_name ()
          << "::_" << node->local_name ()
          << "_forany &);";
    }
  else
    {
      *os << node->name () << "_forany &);";
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
