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
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the CDR << and >> operator declarations
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator<< (TAO_OutputCDR &, const "; 
  // @@ TODO: this should be done in the node, it is absurd to repeat
  // this code all over the visitors!!!!
  if (!this->ctx_->tdef ())
    {
      be_scope* scope = be_scope::narrow_from_scope (node->defined_in ());
      be_decl* parent = scope->decl ();
      *os << parent->fullname ()
          << "::_" << node->local_name ()
          << "_forany &);" << be_nl;
    }
  else
    {
      *os << node->name () << "_forany &);" << be_nl;
    }
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator>> (TAO_InputCDR &, ";
  if (!this->ctx_->tdef ())
    {
      be_scope* scope = be_scope::narrow_from_scope (node->defined_in ());
      be_decl* parent = scope->decl ();
      *os << parent->fullname ()
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
