//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for a valuetype in the client
//    header.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype,
           any_op_ch,
           "$Id$")

// ***************************************************************************
// Valuetype visitor for generating Any operator declarations.
// ***************************************************************************

be_visitor_valuetype_any_op_ch::be_visitor_valuetype_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_any_op_ch::~be_visitor_valuetype_any_op_ch (void)
{
}

int
be_visitor_valuetype_any_op_ch::visit_valuetype (be_valuetype *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  be_module *module = 0;

  if (node->is_nested () &&
      node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      module = be_module::narrow_from_scope (node->defined_in ());

      if (!module)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_valuebox_any_op_ch::"
                             "visit_valuebox - "
                             "Error parsing nested name\n"),
                            -1);
        }

      // Some compilers handle "any" operators in a namespace
      // corresponding to their module, others do not.
      *os << "\n\n#if defined (ACE_ANY_OPS_USE_NAMESPACE)\n";

      be_util::gen_nested_namespace_begin (os, module);

      // emit  nested variation of any operators
      *os << macro << " void"
          << " operator<<= ( ::CORBA::Any &, " << node->local_name ()
          << " *); // copying" << be_nl;

      *os << macro << " void"
          << " operator<<= ( ::CORBA::Any &, " << node->local_name ()
          << " **); // non-copying" << be_nl;

      *os << macro << " ::CORBA::Boolean"
          << " operator>>= (const ::CORBA::Any &, "
          << node->local_name () << " *&);";

      be_util::gen_nested_namespace_end (os, module);

      // emit #else
      *os << "#else\n\n";
    }

  *os << be_global->core_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << " *); // copying" << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << " **); // non-copying" << be_nl;

  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << " *&);";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_valuetype_any_op_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

