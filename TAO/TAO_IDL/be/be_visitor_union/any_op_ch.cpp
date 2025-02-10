
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Any operators for Union.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union_any_op_ch::be_visitor_union_any_op_ch (
    be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_any_op_ch::~be_visitor_union_any_op_ch ()
{
}

int
be_visitor_union_any_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  TAO_INSERT_COMMENT (os);

  *os << be_global->anyops_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, const "
      << node->name () << " *&);";

  *os << be_global->anyops_versioning_end () << be_nl;

  be_visitor_context ctx (*this->ctx_);
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_localtypes);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      be_enum *e = dynamic_cast<be_enum*> (d);
      if (e != nullptr)
        {
          be_visitor_enum_any_op_ch visitor (&ctx);

          if (e->accept (&visitor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%N:%l) be_visitor_union_any_op_ch::visit_union"
                          " - codegen for enum failed\n"));
            }

          // Restore the union node in the enum visitor's context.
          ctx.node (this->ctx_->node ());
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_hdr_any_op_gen (true);
  return 0;
}

int
be_visitor_union_any_op_ch::visit_union_branch (be_union_branch *node)
{
  // First generate the type information.
  be_type *bt = dynamic_cast<be_type*> (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_union_branch - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_any_op_ch::visit_enum (be_enum *node)
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
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_enum - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_any_op_ch::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_structure - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}
