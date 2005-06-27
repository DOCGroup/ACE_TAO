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
                             "be_visitor_valuetype_any_op_ch::"
                             "visit_valuetype - "
                             "Error parsing nested name\n"),
                            -1);
        }
      be_util::gen_nested_namespace_begin (os, module);
    }

  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->local_name ()
      << " *); // copying" << be_nl;

  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->local_name ()
      << " **); // non-copying" << be_nl;

  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, "
      << node->local_name () << " *&);";

  if (module != 0)
    {
      be_util::gen_nested_namespace_end (os, module);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_valuetype_any_op_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

