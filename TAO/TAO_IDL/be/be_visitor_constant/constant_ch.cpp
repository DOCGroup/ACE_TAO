
//=============================================================================
/**
 *  @file    constant_ch.cpp
 *
 *  Visitor generating code for the Constant node in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "constant.h"

be_visitor_constant_ch::be_visitor_constant_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_constant_ch::~be_visitor_constant_ch ()
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

  TAO_INSERT_COMMENT (os);

  // If we are defined in the outermost scope, then the value is assigned
  // to us here itself, else it will be in the *.cpp file.

  AST_Decl::NodeType nt = AST_Decl::NT_pre_defined;
  AST_Decl *tdef = node->constant_value ()->get_tdef ();
  AST_Decl::NodeType bnt = AST_Decl::NT_pre_defined;
  AST_Expression::ExprType etype = node->et ();
  AST_Decl::NodeType snt = node->defined_in ()->scope_node_type ();

  if (tdef != nullptr)
    {
      nt = tdef->node_type ();
      be_typedef *td = dynamic_cast<be_typedef*> (tdef);
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
          *os << exprtype_to_cpp_corba_type (node->et ());
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
          *os << "extern " << be_global->stub_export_macro () << " ";
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
              *os << exprtype_to_cpp_corba_type (node->et ());
            }
          else
            {
              *os << tdef->name ();
            }
        }
      else
        {
          *os << exprtype_to_cpp_corba_type (node->et ());
        }

      *os << " " << node->local_name ();
    }

  // If this is true, can't generate inline constants.
  bool const forbidden_in_class = (snt != AST_Decl::NT_root
                             && snt != AST_Decl::NT_module
                             && (etype == AST_Expression::EV_string
                                 || etype == AST_Expression::EV_wstring
                                 || etype == AST_Expression::EV_float
                                 || etype == AST_Expression::EV_double
                                 || etype == AST_Expression::EV_longdouble));

  if (!node->is_nested ()
      || (be_global->gen_inline_constants () && !forbidden_in_class))
    {
      *os << " = " << node->constant_value ();
    }

  *os << ";";

  node->cli_hdr_gen (true);
  return 0;
}
