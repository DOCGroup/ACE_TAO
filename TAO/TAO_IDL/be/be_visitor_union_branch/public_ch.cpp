
//=============================================================================
/**
 *  @file    public_ch.cpp
 *
 *  Visitor generating code for Union_branch in the public part.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "union_branch.h"

// **********************************************
//  Visitor for union_branch in the client header file.
// **********************************************

be_visitor_union_branch_public_ch::be_visitor_union_branch_public_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_branch_public_ch::~be_visitor_union_branch_public_ch ()
{
}

int
be_visitor_union_branch_public_ch::visit_union_branch (be_union_branch *node)
{
  be_type *bt = dynamic_cast<be_type*> (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types  that a union_branch can be.

int
be_visitor_union_branch_public_ch::visit_array (be_array *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_array - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      // This is the case of an anonymous array inside a union.

      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
      be_visitor_array_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_array - "
                             "codegen failed\n"
                             ),
                            -1);
        }

      ctx.state (TAO_CodeGen::TAO_ROOT_CH);

      TAO_INSERT_COMMENT (os);

      // Now use this array as a "type" for the subsequent declarator
      // the set method.
      *os << be_nl_2
          << "void " << ub->local_name () << " ("
          << "_" << bt->local_name () << ");" << be_nl;
      // The get method.
      *os << "_" << bt->local_name () << "_slice * " << ub->local_name ()
          << " () const; // get method";
    }
  else
    {
      TAO_INSERT_COMMENT (os);

      // Now use this array as a "type" for the subsequent declarator
      // the set method.
      *os << be_nl_2
          << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ");"
          << be_nl;
      // The get method.
      *os << bt->nested_type_name (bu, "_slice *") << " " << ub->local_name ()
          << " () const;";
    }

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_enum (be_enum *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_enum_ch visitor (&ctx);

     if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }
    }

  TAO_INSERT_COMMENT (os);

  // Now use this enum as a "type" for the subsequent declarator
  // the set method.
  *os << be_nl_2
      << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu) << ");"
      << be_nl;
  // the get method.
  *os << bt->nested_type_name (bu) << " " << ub->local_name ()
      << " () const;";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_interface (be_interface *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Set method.
  *os << be_nl_2
      << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ");" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " () const;";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Set method.
  *os << be_nl_2
      << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ");" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " () const;";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_valuebox (be_valuebox *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_valuebox - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Set method.
  *os << be_nl_2
      << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "*")
      << ");" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "*") << " " << ub->local_name ()
      << " () const;";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_valuetype (be_valuetype *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_valuetype - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Set method.
  *os << be_nl_2
      << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "*")
      << ");" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "*") << " " << ub->local_name ()
      << " () const;";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_valuetype_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Set method.
  *os << be_nl_2
      << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "*")
      << ")" << be_nl;
  // Get method.
  *os << bt->nested_type_name (bu, "*") << " " << ub->local_name ()
      << " () const;";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_predefined_type (be_predefined_type *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;
  be_typedef *td = this->ctx_->alias ();

  // Check if we are visiting this via a visit to a typedef node.
  if (td != nullptr)
    {
      bt = td;
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
    case AST_PredefinedType::PT_abstract:
      *os << be_nl_2
          << "void " << ub->local_name () << " (const "
          << bt->nested_type_name (bu, "_ptr") << ");" << be_nl;
      *os << bt->nested_type_name (bu, "_ptr") << " "
          << ub->local_name () << " () const;";
      break;
    case AST_PredefinedType::PT_value:
      *os << be_nl_2
          << "void " << ub->local_name () << " ( "
          << bt->nested_type_name (bu, " *") << ");" << be_nl;
      *os << bt->nested_type_name (bu, " *") << " "
          << ub->local_name () << " () const;";
      break;
    case AST_PredefinedType::PT_any:
      *os << be_nl_2
          << "void " << ub->local_name () << " (const "
          << bt->nested_type_name (bu) << " &);" << be_nl;
      *os << "const " << bt->nested_type_name (bu) << " &"
          << ub->local_name () << " () const;" << be_nl;
      *os << bt->nested_type_name (bu) << " &"
          << ub->local_name () << " ();";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << be_nl_2
          << "void " << ub->local_name () << " ( "
          << bt->nested_type_name (bu) << ");" << be_nl;
      *os << bt->nested_type_name (bu) << " "
          << ub->local_name () << " () const;";
    }

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_sequence (be_sequence *node)
{
  return visit_seq_map_common (node);
}

int
be_visitor_union_branch_public_ch::visit_map (be_map *node)
{
  return visit_seq_map_common (node);
}

int
be_visitor_union_branch_public_ch::visit_seq_map_common (be_type *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_seq_map_common - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      const char* kind = nullptr;
      switch (node->node_type ())
      {
        case AST_Decl::NT_sequence:
          {
            be_visitor_sequence_ch visitor (&ctx);
            if (node->accept (&visitor) == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_union_branch_public_ch::"
                                 "visit_seq_map_common - "
                                 "sequence codegen failed\n"),
                                -1);
            kind = "seq";
          }
          break;
        case AST_Decl::NT_map:
          {
            be_visitor_map_ch visitor (&ctx);
            if (node->accept (&visitor) == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_union_branch_public_ch::"
                                 "visit_seq_map_common - "
                                 "map codegen failed\n"),
                                -1);
            kind = "map";
          }
          break;
        default:
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_visitor_union_branch_public_ch::"
                               "visit_seq_map_common - "
                               "invalid type: %C\n", node->node_type_name ()),
                              -1);
          }
      };

      TAO_INSERT_COMMENT (os);

      // Generate the anonymous sequence member typedef.
      // This provides a consistent name to use instead of the
      // implementation-specific name.
      *os << be_nl_2
          << "typedef " << bt->nested_type_name (bu)
          << " _" << ub->local_name () << "_" << kind << ";";
    }

  TAO_INSERT_COMMENT (os);

  *os << be_nl_2
      << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);" << be_nl;
  *os << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " () const;"
      << be_nl;
  *os << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " ();";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_string (be_string *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Three methods to set the string value
  if (node->width () == (long) sizeof (char))
    {
      *os << be_nl_2
          << "void " << ub->local_name () << " (char *);" << be_nl;
      *os << "void " << ub->local_name () << " (const char *);"
          << be_nl;
      *os << "void " << ub->local_name () << " (const ::CORBA::String_var&);"
          << be_nl;
      *os << "const char *" << ub->local_name ()
          << " () const;";
    }
  else
    {
      *os << be_nl_2
          << "void " << ub->local_name () << " (::CORBA::WChar *);" << be_nl;
      *os << "void " << ub->local_name () << " (const ::CORBA::WChar *);"
          << be_nl;
      *os << "void " << ub->local_name () << " (const ::CORBA::WString_var&);"
          << be_nl;
      *os << "const ::CORBA::WChar *" << ub->local_name ()
          << " () const;";
    }

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_structure (be_structure *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_structure - "
                             "codegen failed\n"),
                            -1);
        }
    }

  TAO_INSERT_COMMENT (os);

  *os << be_nl_2
      << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);" << be_nl
      << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " () const;"
      << be_nl
      << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " ();";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    dynamic_cast<be_structure*> (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_union_branch_public_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_spec_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (nullptr);
  return 0;
}

int
be_visitor_union_branch_public_ch::visit_union (be_union *node)
{
  be_decl *ub = this->ctx_->node ();
  be_decl *bu = this->ctx_->scope ()->decl ();
  be_type *bt = nullptr;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ch::"
                         "visit_union - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Not a typedef and bt is defined inside the union.
  if (bt->node_type () != AST_Decl::NT_typedef
      && bt->is_child (bu))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_ch::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  TAO_INSERT_COMMENT (os);

  *os << be_nl_2
      << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &);" << be_nl
      << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " () const;"
      << be_nl
      << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " ();";

  return 0;
}

int
be_visitor_union_branch_public_ch::visit_union_fwd (be_union_fwd *node)
{
  be_union *u =
    dynamic_cast<be_union*> (node->full_definition ());

  return this->visit_union (u);
}

