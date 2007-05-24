//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    constant_cs.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Constant code in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_constant,
           constant_cs,
           "$Id$")


// ********************************************************************
// Visitor implementation for the Constant type
// This one for the client stubs file
// ********************************************************************

be_visitor_constant_cs::be_visitor_constant_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_constant_cs::~be_visitor_constant_cs (void)
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
      // No storeage is required in the source file for these
      return 0;
    }

  AST_Expression::ExprType etype = node->et ();
  bool forbidden_in_class = (in_class
                             && (etype == AST_Expression::EV_string
                                 || etype == AST_Expression::EV_wstring
                                 || etype == AST_Expression::EV_float
                                 || etype == AST_Expression::EV_double
                                 || etype == AST_Expression::EV_longdouble));

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl;
  if (be_global->gen_inline_constants () && !forbidden_in_class)
    {
      // Some compilers (especially Microsoft) always define storeage
      // when the initialiser is given, therefore the header has already
      // done this (which is non-C++ complient by the way).
      *os << "#ifndef ACE_HAS_HEADER_ALLOCATED_CLASS_STATIC_CONST_INT_STOREAGE" << be_nl << "  ";
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

  *os << " " << node->name ();
  if (!be_global->gen_inline_constants () || forbidden_in_class)
    {
      // For those constants not defined in the outermost scope,
      // or in a module, they get assigned to their values in the source file.
      *os << " = " << node->constant_value ();
    }
  *os << ";";

  if (be_global->gen_inline_constants () && !forbidden_in_class)
    {
      *os << be_nl << "#endif // ifndef ACE_HAS_HEADER_ALLOCATED_CLASS_STATIC_CONST_INT_STOREAGE";
    }

  node->cli_stub_gen (true);
  return 0;
}

// The following needs to be done until the MSVC++ compiler fixes its
// broken handling of namespaces (hopefully forthcoming in version 7).
int
be_visitor_constant_cs::gen_nested_namespace_begin (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  char *item_name = 0;

  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      item_name = i.item ()->get_string ();

      if (ACE_OS::strcmp (item_name, "") != 0)
        {
          // leave the outermost root scope.
          *os << "namespace " << item_name << be_nl
              << "{" << be_nl;
        }
    }

  return 0;
}

// The following needs to be done until the MSVC++ compiler fixes its
// broken handling of namespaces (hopefully forthcoming in version 7).
int
be_visitor_constant_cs::gen_nested_namespace_end (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      if (ACE_OS::strcmp (i.item ()->get_string (), "") != 0)
        {
          // leave the outermost root scope.
          *os << "}" << be_nl;
        }
    }

  return 0;
}
