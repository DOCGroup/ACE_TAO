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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_array.h"
#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_array, cdr_op_ch, "$Id$")


// ***************************************************************************
// Array visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_array_cdr_op_ch::be_visitor_array_cdr_op_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_cdr_op_ch::~be_visitor_array_cdr_op_ch (void)
{
}

int
be_visitor_array_cdr_op_ch::visit_array (be_array *node)
{
  if (node->cli_hdr_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  AST_Type *bt = node->base_type ();
  AST_Decl::NodeType nt = bt->node_type ();

  // If the node is an array of anonymous sequence, we need to
  // generate the sequence's cdr operator declaration here.
  if (nt == AST_Decl::NT_sequence && bt->anonymous ())
    {
      be_sequence *bs = be_sequence::narrow_from_decl (bt);
      be_visitor_sequence_cdr_op_ch visitor (this->ctx_);

      if (bs->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_cdr_op_ch::"
                             "visit_array - "
                             "accept on anonymous base type failed\n"),
                            -1);
        }
    }

  // Generate the CDR << and >> operator declarations.
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator<< (TAO_OutputCDR &, const ";
  // @@ TODO: this should be done in the node, it is absurd to repeat
  // this code all over the visitors!!!!
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
          << "_forany &);" << be_nl;
    }
  else
    {
      *os << node->name () << "_forany &);" << be_nl;
    }

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
