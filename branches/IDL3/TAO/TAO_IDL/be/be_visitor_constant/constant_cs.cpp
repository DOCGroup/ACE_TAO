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
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_stub_gen () 
      && !node->imported ()
      && !be_global->gen_inline_constants ())
    {
      if (node->is_nested ())
        {
          if (node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
            {
              *os << "TAO_NAMESPACE_TYPE (const "
                  << node->exprtype_to_string () << ")" << be_nl;
              be_module *module = 
                be_module::narrow_from_scope (node->defined_in ());

              if (!module || this->gen_nested_namespace_begin (module) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "be_visitor_constant_cs::"
                                     "visit_constant - "
                                     "Error parsing nested name\n"),
                                    -1);
                }

              *os << "TAO_NAMESPACE_DEFINE (const ";

              if (node->et () == AST_Expression::EV_any)
                {
                  *os << node->enum_full_name ();
                }
              else
                {
                  *os << node->exprtype_to_string ();
                }

              *os << ", " << node->local_name () << ", "
                  << node->constant_value () << ")" << be_nl;

              if (this->gen_nested_namespace_end (module) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "be_visitor_constant_cs::"
                                     "visit_constant - "
                                     "Error parsing nested name\n"),
                                    -1);
                }
            }
          else
            {
              // For those constants not defined in the outer most scope,
              // they get assigned to their values in the impl file.
              os->indent ();

              *os << "const " << node->exprtype_to_string () << " "
                  << node->name () << " = " << node->constant_value ()
                  << ";\n\n";
            }
        }

      node->cli_stub_gen (I_TRUE);
    }

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
          *os << "TAO_NAMESPACE_BEGIN (" << item_name
              << ")" << be_nl;
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
          *os << "TAO_NAMESPACE_END" << be_nl;
        }
    }

  return 0;
}
