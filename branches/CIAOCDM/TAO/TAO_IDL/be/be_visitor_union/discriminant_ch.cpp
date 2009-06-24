//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    discriminant_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for discriminant of the Union
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union,
           discriminant_ch,
           "$Id$")

// *************************************************************************
// Visitor for discriminant in client header file.
// *************************************************************************

be_visitor_union_discriminant_ch::be_visitor_union_discriminant_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ch::~be_visitor_union_discriminant_ch (void)
{
}

int
be_visitor_union_discriminant_ch::visit_enum (be_enum *node)
{
  // Get the enclosing union backend.
  be_union *bu = this->ctx_->be_node_as_union ();
  be_type *bt;

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

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // The set method.
  *os << be_nl << be_nl
      << "void _d (" << bt->nested_type_name (bu) << ");" << be_nl;
  // The get method.
  *os << bt->nested_type_name (bu) << " _d (void) const;";

  return 0;
}

int
be_visitor_union_discriminant_ch::visit_predefined_type (be_predefined_type
                                                         *node)
{
  // get the enclosing union backend.
  be_union *bu = this->ctx_->be_node_as_union ();
  be_type *bt;

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

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // The set method.
  *os << be_nl << be_nl
      << "void _d ( " << bt->nested_type_name (bu) << ");" << be_nl;
  // The get method.
  *os << bt->nested_type_name (bu) << " _d (void) const;";

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

  this->ctx_->alias (0);
  return 0;
}
