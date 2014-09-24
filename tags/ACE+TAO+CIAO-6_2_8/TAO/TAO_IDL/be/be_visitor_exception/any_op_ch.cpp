
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Any operators for Exceptions
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "exception.h"

be_visitor_exception_any_op_ch::be_visitor_exception_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_exception_any_op_ch::~be_visitor_exception_any_op_ch (void)
{
}

int
be_visitor_exception_any_op_ch::visit_exception (be_exception *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

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

          *os << be_nl_2
              << macro << " void operator<<= (::CORBA::Any &, const ::" << node->name ()
              << " &); // copying version" << be_nl;
          *os << macro << " void operator<<= (::CORBA::Any &, ::" << node->name ()
              << "*); // noncopying version" << be_nl;
          *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, ::"
              << node->name () << " *&); // deprecated\n";
          *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, const ::"
              << node->name () << " *&);";

          be_util::gen_nested_namespace_end (os, module);

          // Emit #else.
          *os << be_nl_2
              << "#else\n\n";
        }
    }

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_nl_2
      << macro << " void operator<<= (::CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << " *&); // deprecated\n";
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, const "
      << node->name () << " *&);";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::visit_exception - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_exception_any_op_ch::visit_field (be_field *node)
{
  // First generate the type information.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_ch::visit_enum (be_enum *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_enum_any_op_ch visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_enum - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_ch::visit_structure (be_structure *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_structure_any_op_ch visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_structure - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_union_any_op_ch visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_ch::"
                         "visit_union - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

