
//=============================================================================
/**
 *  @file    field_ch.cpp
 *
 *  $Id$
 *
 *   Visitor for the structure fields in valuebox class.
 *   This one generates code for access to strucuture members
 *   in the client header.
 *
 *   Based on be_visitor_union_branch/public_ch.  In general we
 *   generate the same method signatures as that visitor but cannot
 *   use it directly because a valuebox is not a scope and because
 *   the structure is visited prior to visiting the valuebox.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================

#include "valuebox.h"

be_visitor_valuebox_field_ch::be_visitor_valuebox_field_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuebox_field_ch::~be_visitor_valuebox_field_ch (void)
{
}

int
be_visitor_valuebox_field_ch::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_field - "
                         "Bad field type\n"
                         ),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_union_branch - "
                         "codegen for valuebox field failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types  that a union_branch can be.

int
be_visitor_valuebox_field_ch::visit_array (be_array *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_array - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();


  char fname [NAMEBUFSIZE];
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  if (bt->node_type () != AST_Decl::NT_typedef)
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parent's name.
      if (bt->is_nested ())
        {
          be_decl *parent =
                 be_scope::narrow_from_scope (bt->defined_in ())->decl ();
          ACE_OS::sprintf (fname,
                           "%s::_%s",
                           parent->full_name (),
                           bt->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname,
                           "_%s",
                           bt->full_name ());
        }
    }
  else
    { // Typedefed node.
      ACE_OS::sprintf (fname,
                       "%s",
                       bt->full_name ());
    }


  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "void " << field->local_name () << " (" << fname << ");" << be_nl;

  *os << "const " << fname << "_slice *" << field->local_name ()
      << " (void) const;" << be_nl;

  return 0;
}

int
be_visitor_valuebox_field_ch::visit_enum (be_enum *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_member_set (field, bt, "", "");
  this->emit_member_get (field, bt, "", "", "const");

  return 0;
}

int
be_visitor_valuebox_field_ch::visit_interface (be_interface *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_member_set (field, bt, "", "_ptr");
  this->emit_member_get (field, bt, "", "_ptr", "const");

  return 0;
}

int
be_visitor_valuebox_field_ch::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_member_set (field, bt, "", "_ptr");
  this->emit_member_get (field, bt, "", "_ptr", "const");

  return 0;
}

int
be_visitor_valuebox_field_ch::visit_valuetype (be_valuetype *)
{
  // Valuetype is not a valid component of a valuebox
  return 0;
}

int
be_visitor_valuebox_field_ch::visit_valuetype_fwd (be_valuetype_fwd *)
{
  // Valuetype is not a valid component of a valuebox
  return 0;
}

int
be_visitor_valuebox_field_ch::visit_predefined_type (be_predefined_type *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      this->emit_member_set (field, bt, " ::", "_ptr");
      this->emit_member_get (field, bt, " ::", "_ptr", "const");

      break;
    case AST_PredefinedType::PT_any:
      this->emit_member_set (field, bt, "const ::", " &");
      this->emit_member_get (field, bt, "const ::", " &", "const");
      this->emit_member_get (field, bt, " ::", " &", "");
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      this->emit_member_set (field, bt, " ::", "");
      this->emit_member_get (field, bt, " ::", "", "const");
    }

  return 0;
}

int
be_visitor_valuebox_field_ch::visit_sequence (be_sequence *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_sequence - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_member_set (field, bt, "const ", " &");
  this->emit_member_get (field, bt, "const ", " &", "const");
  this->emit_member_get (field, bt, "", " &", "");


  return 0;
}

int
be_visitor_valuebox_field_ch::visit_string (be_string *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_string - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_member_set (field, bt, "", "");
  this->emit_member_set (field, bt, "const ", "");

  const char *string_type = "";
  if (node->node_type () == AST_Decl::NT_string)
    {
      string_type = "String";
    }
  else if (node->node_type () == AST_Decl::NT_wstring)
    {
      string_type = "WString";
    }

  *os << "void " << field->local_name ()
      << " (const ::CORBA::" << string_type << "_var &);" << be_nl;

  this->emit_member_get (field, bt, "const ", "", "const");

  return 0;
}

int
be_visitor_valuebox_field_ch::visit_structure (be_structure *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_member_set (field, bt, "const ", " &");
  this->emit_member_get (field, bt, "const ", " &", "const");
  this->emit_member_get (field, bt, "", " &", "");

  return 0;
}

int
be_visitor_valuebox_field_ch::visit_typedef (be_typedef *node)
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

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_valuebox_field_ch::visit_union (be_union *node)
{
  be_decl *field = this->ctx_->node ();
  be_type *bt = 0;

  // Check if we are visiting this via a visit to a typedef node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!field)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_field_ch::"
                         "visit_union - "
                         "bad context information\n"
                         ),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();


  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  this->emit_member_set (field, bt, "const ", " &");
  this->emit_member_get (field, bt, "const ", " &", "const");
  this->emit_member_get (field, bt, "", " &", "");

  return 0;
}


void
be_visitor_valuebox_field_ch::emit_member_set (be_decl *field,
                                               be_type *field_type,
                                               const char *const_arg,
                                               const char *arg_modifier)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "void " << field->local_name ()
      << " (" << const_arg << field_type->name () << arg_modifier << ");"
      << be_nl;

}


void
be_visitor_valuebox_field_ch::emit_member_get (be_decl *field,
                                               be_type *field_type,
                                               const char *const_prefix,
                                               const char *type_suffix,
                                               const char *const_method)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << const_prefix << field_type->name () << type_suffix << " "
      << field->local_name () << " (void) " << const_method << ";" << be_nl;
}
