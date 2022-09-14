
//=============================================================================
/**
 *  @file    constant_cs.cpp
 *
 *  Visitor for code generation of Constant code in the client stubs file.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "constant.h"

be_visitor_constant_cs::be_visitor_constant_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_constant_cs::~be_visitor_constant_cs ()
{
}

// visit the Constant_cs node and its scope
int
be_visitor_constant_cs::visit_constant (be_constant *node)
{
  if (node->cli_stub_gen ()
      || node->imported ()
      || !node->is_nested ())
    {
      return 0;
    }

  AST_Decl::NodeType snt = node->defined_in ()->scope_node_type ();
  bool in_class = (snt != AST_Decl::NT_root
                   && snt != AST_Decl::NT_module);
  if (be_global->gen_inline_constants () && !in_class)
    {
      // No storage is required in the source file for these
      return 0;
    }

  AST_Expression::ExprType etype = node->et ();
  bool const forbidden_in_class = (in_class
                             && (etype == AST_Expression::EV_string
                                 || etype == AST_Expression::EV_wstring
                                 || etype == AST_Expression::EV_float
                                 || etype == AST_Expression::EV_double
                                 || etype == AST_Expression::EV_longdouble));

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  *os << be_nl_2;
  *os << "const ";

  if (node->et () == AST_Expression::EV_enum)
    {
      *os << node->enum_full_name ();
    }
  else
    {
      *os << exprtype_to_cpp_corba_type (node->et ());
    }

  *os << " " << node->name ();
  if (!be_global->gen_inline_constants () || forbidden_in_class)
    {
      // For those constants not defined in the outermost scope,
      // or in a module, they get assigned to their values in the source file.
      *os << " = " << node->constant_value ();
    }
  *os << ";";

  node->cli_stub_gen (true);
  return 0;
}
