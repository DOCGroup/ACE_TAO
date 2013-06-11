
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Any operators for an Interface in the client
 *  header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_any_op_ch::be_visitor_interface_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_any_op_ch::~be_visitor_interface_any_op_ch (void)
{
}

int
be_visitor_interface_any_op_ch::visit_interface (be_interface *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ()
      || (node->is_local () && !be_global->gen_local_iface_anyops ()))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  be_module *module = 0;

  if (node->is_nested ())
    {
      AST_Decl *d = node;
      AST_Decl::NodeType nt = d->node_type ();

      while (nt != AST_Decl::NT_root)
        {
          if (nt == AST_Decl::NT_module)
            {
              module = be_module::narrow_from_decl (d);
              break;
            }
          else
            {
              d = ScopeAsDecl (d->defined_in ());
              nt = d->node_type ();
            }
        }

      if (module != 0)
        {
          // Some compilers handle "any" operators in a namespace
          // corresponding to their module, others do not.
          *os << "\n\n#if defined (ACE_ANY_OPS_USE_NAMESPACE)\n";

          be_util::gen_nested_namespace_begin (os, module);

          // emit  nested variation of any operators
          *os << macro << " void"
              << " operator<<= ( ::CORBA::Any &, " << node->local_name ()
              << "_ptr); // copying" << be_nl;
          *os << macro << " void"
              << " operator<<= ( ::CORBA::Any &, " << node->local_name ()
              << "_ptr *); // non-copying" << be_nl;
          *os << macro << " ::CORBA::Boolean"
              << " operator>>= (const ::CORBA::Any &, "
              << node->local_name () << "_ptr &);";

          be_util::gen_nested_namespace_end (os, module);

          // Emit #else.
          *os << be_nl_2
              << "#else\n\n";
        }
    }

  *os << be_global->core_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << "_ptr); // copying" << be_nl;
  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << "_ptr *); // non-copying" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << "_ptr &);";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_any_op_ch::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }


  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_interface_any_op_ch::visit_component (
  be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_any_op_ch::visit_connector (
  be_connector *node)
{
  return this->visit_interface (node);
}

