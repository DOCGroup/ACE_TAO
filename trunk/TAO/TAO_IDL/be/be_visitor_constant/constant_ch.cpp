//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    constant_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for the Constant node in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_constant, 
           constant_ch, 
           "$Id$")


// ********************************************************************
// Visitor implementation for the Constant type
// This one for the client header file
// ********************************************************************

be_visitor_constant_ch::be_visitor_constant_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_constant_ch::~be_visitor_constant_ch (void)
{
}

// Visit the Constant node and its scope.
int
be_visitor_constant_ch::visit_constant (be_constant *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // If we are defined in the outermost scope, then the value is assigned
  // to us here itself, else it will be in the *.cpp file.

//  if (be_global->gen_inline_constants ())
  if (! node->is_nested ()
      || node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      *os << "const ";

      if (node->et () == AST_Expression::EV_enum)
        {
          *os << node->enum_full_name ();
        }
      else
        {
          *os << node->exprtype_to_string ();
        }

      *os << " " << node->local_name () << " = "
          << node->constant_value ();
    }
  // We are nested inside an interface or a valuetype.
  else 
    {
      *os << "static const ";

      if (node->et () == AST_Expression::EV_enum)
        {
          *os << node->enum_full_name ();
        }
      else
        {
          *os << node->exprtype_to_string ();
        }

      *os << " " << node->local_name ();

      if (!node->is_nested ())
        {
          // We were defined at the outermost scope. So we put the value
          // in the header itself.
          *os << " = " << node->constant_value ();
        }
    }

  *os << ";" << be_nl << be_nl;

  node->cli_hdr_gen (I_TRUE);
  return 0;
}
