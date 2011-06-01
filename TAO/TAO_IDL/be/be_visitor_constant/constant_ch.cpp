
//=============================================================================
/**
 *  @file    constant_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Constant node in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // If we are defined in the outermost scope, then the value is assigned
  // to us here itself, else it will be in the *.cpp file.

  AST_Decl::NodeType nt = AST_Decl::NT_pre_defined;
  AST_Decl *tdef = node->constant_value ()->get_tdef ();
  AST_Decl::NodeType bnt = AST_Decl::NT_pre_defined;
  AST_Expression::ExprType etype = node->et ();
  AST_Decl::NodeType snt = node->defined_in ()->scope_node_type ();

  if (tdef != 0)
    {
      nt = tdef->node_type ();
      be_typedef *td = be_typedef:: narrow_from_decl (tdef);
      bnt = td->base_node_type ();
    }

  *os << be_nl_2;

  if (! node->is_nested ())
    {
      *os << "const ";

      if (etype == AST_Expression::EV_enum)
        {
          *os << node->enum_full_name ();
        }
      else if (nt == AST_Decl::NT_typedef)
        {
          *os << tdef->name ();
        }
      else
        {
          *os << node->exprtype_to_string ();
        }

      *os << " " << node->local_name ();
    }
  // We are nested inside an interface or a valuetype.
  else
    {
      if (snt != AST_Decl::NT_module)
        {
          *os << "static ";
        }
      else if (!be_global->gen_inline_constants ())
        {
          *os << "TAO_NAMESPACE_STORAGE_CLASS ";
        }

      *os << "const ";

      if (etype == AST_Expression::EV_enum)
        {
          *os << node->enum_full_name ();
        }
      else if (nt == AST_Decl::NT_typedef)
        {
          if (bnt == AST_Decl::NT_string || bnt == AST_Decl::NT_wstring)
            {
              *os << node->exprtype_to_string ();
            }
          else
            {
              *os << tdef->name ();
            }
        }
      else
        {
          *os << node->exprtype_to_string ();
        }

      *os << " " << node->local_name ();
    }

  // If this is true, can't generate inline constants.
  bool forbidden_in_class = (snt != AST_Decl::NT_root
                             && snt != AST_Decl::NT_module
                             && (etype == AST_Expression::EV_string
                                 || etype == AST_Expression::EV_wstring
                                 || etype == AST_Expression::EV_float
                                 || etype == AST_Expression::EV_double
                                 || etype == AST_Expression::EV_longdouble));

  if (!node->is_nested ()
      || (be_global->gen_inline_constants () && !forbidden_in_class))
    {
#if defined (NONNATIVE_LONGDOUBLE)
      if (etype == AST_Expression::EV_longdouble)
        {
          *os << " (" << node->constant_value () << ")";
        }
      else
        {
          *os << " = " << node->constant_value ();
        }
#else
      *os << " = " << node->constant_value ();
#endif // defined (NONNATIVE_LONGDOUBLE)
    }

  *os << ";";

  node->cli_hdr_gen (true);
  return 0;
}
