
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Any operators for a forward declared interface
 *  in the client header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ***************************************************************************
// Generates Any operator declarations in the client header
// ***************************************************************************

be_visitor_interface_fwd_any_op_ch::be_visitor_interface_fwd_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_any_op_ch::~be_visitor_interface_fwd_any_op_ch (void)
{
}

int
be_visitor_interface_fwd_any_op_ch::visit_interface_fwd (
    be_interface_fwd *node
  )
{
  // Only a forward declared interface that is not defined in the same
  // translation unit needs to have this generated here. The Any operators
  // are needed by portable interceptor code if the interface is a
  // parameter of an operation.
  if (node->full_def_seen () || node->is_local ())
    {
      return 0;
    }

  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  be_module *module = 0;

  if (node->is_nested () &&
      node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      module = be_module::narrow_from_scope (node->defined_in ());

      if (0 == module)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_valuebox_any_op_ch::"
                             "visit_interface_fwd - "
                             "Error parsing nested name\n"),
                            -1);
        }

      // Some compilers handle "any" operators in a namespace
      // corresponding to their module, others do not.
      *os << "\n\n#if defined (ACE_ANY_OPS_USE_NAMESPACE)\n";

      be_util::gen_nested_namespace_begin (os, module);

      *os << macro << " void"
          << " operator<<= ( ::CORBA::Any &, " << node->local_name ()
          << "_ptr); // copying" << be_nl;
      *os << macro << " void"
          << " operator<<= ( ::CORBA::Any &, " << node->local_name ()
          << "_ptr *); // non-copying" << be_nl;
      *os << macro << " ::CORBA::Boolean"
          << " operator>>= (const ::CORBA::Any &, "
          << node->local_name () << " *&);";

      be_util::gen_nested_namespace_end (os, module);

      // Emit #else.
      *os << be_nl_2
          << "#else\n\n";
    }

  *os << be_global->core_versioning_begin () << be_nl;

  *os << macro << " void"
      << " operator<<= (::CORBA::Any &, " << node->name ()
      << "_ptr); // copying" << be_nl;
  *os << macro << " void"
      << " operator<<= (::CORBA::Any &, " << node->name ()
      << "_ptr *); // non-copying" << be_nl;
  *os << macro << " ::CORBA::Boolean"
      << " operator>>= (const ::CORBA::Any &, "
      << node->name () << " *&);";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}
