
//=============================================================================
/**
 *  @file    discriminant_ch.cpp
 *
 *  Visitor generating code for discriminant of the Union
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union.h"

be_visitor_union_discriminant_ch::be_visitor_union_discriminant_ch (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ch::~be_visitor_union_discriminant_ch ()
{
}

int
be_visitor_union_discriminant_ch::visit_enum (be_enum *node)
{
  // Get the enclosing union backend.
  be_union *bu =
    dynamic_cast<be_union*> (this->ctx_->node ());
  be_type *bt = nullptr;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_OutStream *os = this->ctx_->stream ();

   // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the enum declaration.
      be_visitor_enum_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_ch::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }
    }

  TAO_INSERT_COMMENT (os);

  // The set method.
  *os << be_nl_2
      << "void _d (" << bt->nested_type_name (bu) << ");" << be_nl;
  // The get method.
  *os << bt->nested_type_name (bu) << " _d () const;";

  return 0;
}

int
be_visitor_union_discriminant_ch::visit_predefined_type (be_predefined_type
                                                         *node)
{
  // get the enclosing union backend.
  be_union *bu =
    dynamic_cast<be_union*> (this->ctx_->node ());
  be_type *bt = nullptr;

  // Check if we are visiting this node via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // The set method.
  *os << be_nl_2
      << "void _d ( " << bt->nested_type_name (bu) << ");" << be_nl;
  // The get method.
  *os << bt->nested_type_name (bu) << " _d () const;";

  return 0;
}

int
be_visitor_union_discriminant_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_discriminant_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (nullptr);
  return 0;
}
