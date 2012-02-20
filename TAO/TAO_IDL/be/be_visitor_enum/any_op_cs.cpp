
//=============================================================================
/**
 *  @file    any_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitors for generation of code for Enum for the Any operators
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "enum.h"

be_visitor_enum_any_op_cs::be_visitor_enum_any_op_cs (
    be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_enum_any_op_cs::~be_visitor_enum_any_op_cs (void)
{
}

int
be_visitor_enum_any_op_cs::visit_enum (be_enum *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  *os << be_global->core_versioning_begin () << be_nl;

  // Since we don't generate CDR stream operators for types that
  // explicitly contain a local interface (at some level), we
  // must override these Any template class methods to avoid
  // calling the non-existent operators. The zero return value
  // will eventually cause CORBA::MARSHAL to be raised if this
  // type is inserted into an Any and then marshaled.
  if (node->is_local ())
    {
      *os << "namespace TAO" << be_nl
          << "{" << be_idt_nl
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Basic_Impl_T<" << node->name ()
          << ">::marshal_value (TAO_OutputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Basic_Impl_T<" << node->name ()
          << ">::demarshal_value (TAO_InputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}" << be_nl_2;
    }


  *os << be_global->core_versioning_end () << be_nl;

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
          // Some compilers handle "any" operators in a namespace corresponding
          // to their module, others do not.
          *os << "\n\n#if defined (ACE_ANY_OPS_USE_NAMESPACE)\n";

          be_util::gen_nested_namespace_begin (os, module);


          // Generate the Any <<= and >>= operator declarations
          // Any <<= and >>= operators.
          *os << "void operator<<= (" << be_idt << be_idt_nl
              << "::CORBA::Any &_tao_any," << be_nl
              << "::" << node->name () << " _tao_elem" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl
              << "TAO::Any_Basic_Impl_T< ::" << node->name () << ">::insert ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          *os << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
              << "const ::CORBA::Any &_tao_any," << be_nl
              << "::" << node->name () << " &_tao_elem" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl
              << "return" << be_idt_nl
              << "TAO::Any_Basic_Impl_T< ::" << node->name () << ">::extract ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem " << be_uidt_nl
              << ");" << be_uidt << be_uidt << be_uidt_nl
              << "}";

          be_util::gen_nested_namespace_end (os, module);

          // Emit #else.
          *os << be_nl_2
              << "#else\n";
        }
    }

  *os << be_global->core_versioning_begin () << be_nl;

  // Generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators.
  *os << "void operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << node->name () << " _tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Basic_Impl_T<" << node->name () << ">::insert ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << node->name () << " &_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "TAO::Any_Basic_Impl_T<" << node->name () << ">::extract ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem " << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}
