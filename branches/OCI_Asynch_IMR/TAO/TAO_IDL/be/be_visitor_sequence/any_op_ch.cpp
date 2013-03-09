
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Any operators for the Sequence
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "sequence.h"

be_visitor_sequence_any_op_ch::be_visitor_sequence_any_op_ch (
                                                              be_visitor_context *ctx
                                                              )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_any_op_ch::~be_visitor_sequence_any_op_ch (void)
{
}

int
be_visitor_sequence_any_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ()
      || (node->is_local ()
          && !be_global->gen_local_iface_anyops ()))
    {
      return 0;
    }

  if (idl_global->dcps_sequence_type_defined (node->full_name ()))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  ACE_CString name;

  bool alt = be_global->alt_mapping ();

  if (alt)
    {
      be_type *bt =
        be_type::narrow_from_decl (node->base_type ());

      name = "std::vector<";
      name += bt->full_name ();
      name += ">";
    }
  else
    {
      name = node->full_name ();
    }

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

          // Generate the Any <<= and >>= operators.
          *os << macro
              << " void"
              << " operator<<= ( ::CORBA::Any &, const ::"
              << name.c_str ()
              << " &); // copying version" << be_nl;

          if (!alt)
            {
              *os << macro
                  << " void"
                  << " operator<<= ( ::CORBA::Any &, ::"
                  << name.c_str ()
                  << "*); // noncopying version" << be_nl;

              *os << macro
                  << " ::CORBA::Boolean"
                  << " operator>>= (const ::CORBA::Any &, ::"
                  << name.c_str ()
                  << " *&); // deprecated" << be_nl;
            }

          *os << macro
              << " ::CORBA::Boolean"
              << " operator>>= (const ::CORBA::Any &, const ::"
              << name.c_str ()
              << " *&);";

          be_util::gen_nested_namespace_end (os, module);

          // Emit #else.
          *os << be_nl_2
              << "#else\n\n";
        }
    }

  *os << be_global->core_versioning_begin () << be_nl;

  // Generate the Any <<= and >>= operators.
  *os << macro
      << " void"
      << " operator<<= ( ::CORBA::Any &, const "
      << name.c_str ()
      << " &); // copying version" << be_nl;

  if (!alt)
    {
      *os << macro
          << " void"
          << " operator<<= ( ::CORBA::Any &, "
          << name.c_str ()
          << "*); // noncopying version" << be_nl;

      *os << macro
          << " ::CORBA::Boolean"
          << " operator>>= (const ::CORBA::Any &, "
          << name.c_str ()
          << " *&); // deprecated" << be_nl;
    }

  *os << macro
      << " ::CORBA::Boolean"
      << " operator>>= (const ::CORBA::Any &, const "
      << name.c_str ()
      << " *&);";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }


  node->cli_hdr_any_op_gen (true);
  return 0;
}
