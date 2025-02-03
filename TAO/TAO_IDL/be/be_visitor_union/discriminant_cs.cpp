
//=============================================================================
/**
 *  @file    discriminant_cs.cpp
 *
 *  Visitor generating code for discriminant of the union
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union_discriminant_cs::be_visitor_union_discriminant_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_cs::~be_visitor_union_discriminant_cs ()
{
}

int
be_visitor_union_discriminant_cs::visit_enum (be_enum *node)
{
  be_union *bu =
    dynamic_cast<be_union*> (this->ctx_->node ());
  be_type *bt = nullptr;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // generate the typecode for the enum
      be_visitor_enum_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_cs::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}
