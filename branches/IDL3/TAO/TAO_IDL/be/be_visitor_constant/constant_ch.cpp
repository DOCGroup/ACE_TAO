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
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      // If we are defined in the outermost scope, then the value is assigned
      // to us here itself, else it will be in the *.cpp file.

      if (be_global->gen_inline_constants ())
        {
          if (node->et () == AST_Expression::EV_enum)
            {
              *os << node->enum_full_name ();
            }
          else
            {
              *os << node->exprtype_to_string ();
            }

          *os << " const "
              << node->local_name () << " = "
              << node->constant_value ();
        }
      // Is our enclosing scope a module? We need this check because for
      // platforms that support namespaces, the constant must be declared
      // extern.
      else 
        {
          AST_Decl::NodeType nt = node->defined_in ()->scope_node_type ();

          if (node->is_nested () && nt == AST_Decl::NT_module)
            {
              *os << "TAO_NAMESPACE_STORAGE_CLASS ";
            }
          else
            {
              *os << "static ";
            }

          *os << "const ";

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
    }

  return 0;
}
