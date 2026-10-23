
//=============================================================================
/**
 *  @file    union_member_ci.cpp
 *
 *   Visitor for the Valuebox class.
 *   This one generates code for accessor and modifier functions of
 *   for valuebox union members.
 *
 *  @author Gary Maxey
 */
//=============================================================================

#include "valuebox.h"

#include <memory>
#include <string>

namespace
{
  void
  emit_union_member_type (TAO_OutStream *os,
                          char const *const_prefix,
                          UTL_IdList *name,
                          char const *type_suffix)
  {
    bool is_const = const_prefix[0] == 'c'
      && const_prefix[1] == 'o'
      && const_prefix[2] == 'n'
      && const_prefix[3] == 's'
      && const_prefix[4] == 't'
      && const_prefix[5] == ' ';

    if (is_const)
      {
        const_prefix += 6;
      }

    std::unique_ptr<char[]> name_string (name->get_string_copy ());
    std::string type_name (const_prefix);
    type_name += name_string.get ();

    if (is_const)
      {
        std::string::size_type star = type_name.find ('*');

        if (star == std::string::npos)
          {
            type_name += " const";
          }
        else
          {
            std::string::size_type position = star;
            while (position > 0 && type_name[position - 1] == ' ')
              {
                --position;
              }

            type_name.insert (position, " const");
            if (position == star)
              {
                type_name.insert (position + 6, " ");
              }
          }
      }

    type_name += type_suffix;
    *os << type_name.c_str ();
  }
}

be_visitor_valuebox_union_member_ci::be_visitor_valuebox_union_member_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuebox_union_member_ci::~be_visitor_valuebox_union_member_ci () = default;

int
be_visitor_valuebox_union_member_ci::visit_union_member (be_union_branch *node)
{
  be_type *bt = dynamic_cast<be_type*> (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_union_member - "
                         "Bad field type\n"),
                        -1);
    }

  this->vb_node_ =
    dynamic_cast<be_valuebox*> (this->ctx_->node ());
  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_union_member - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types that a union member can be

int
be_visitor_valuebox_union_member_ci::visit_array (be_array *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
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
                 dynamic_cast<be_scope*> (bt->defined_in ())->decl ();
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
    {
      // Typedefed node.
      ACE_OS::sprintf (fname,
                       "%s",
                       bt->full_name ());
    }

  TAO_INSERT_COMMENT (os);

  *os << "/// Modifier to set the member." << be_nl;

  *os << "ACE_INLINE void" << be_nl
      << vb_node_->name () << "::" << field->local_name ()
      << " (" << fname << " val)" << be_nl
      << "{" << be_idt_nl
      << "this->_pd_value->" << field->local_name ()
      << " (val);" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "/// Accessor to retrieve the member." << be_nl;

  *os << "ACE_INLINE " << fname << "_slice const *" << be_nl
      << vb_node_->name () << "::" << field->local_name ()
      << " () const" << be_nl
      << "{" << be_idt_nl
      << "return this->_pd_value->" << field->local_name ()
      << " ();" << be_uidt_nl
      << "}" << be_nl_2;

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_enum (be_enum *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_enum - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  this->emit_member_set (field, bt, "", "");
  this->emit_member_get (field, bt, "", "", "const");

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_interface (be_interface *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_interface - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  this->emit_member_set (field, bt, "", "_ptr");
  this->emit_member_get (field, bt, "", "_ptr", "const");

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_interface_fwd (be_interface_fwd *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_interface_fwd - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  this->emit_member_set (field, bt, "", "_ptr");
  this->emit_member_get (field, bt, "", "_ptr", "const");

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_valuetype (be_valuetype *)
{
  // Valuetype is not a valid component of a valuebox
  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_valuetype_fwd (be_valuetype_fwd *)
{
  // Valuetype is not a valid component of a valuebox
  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_predefined_type (be_predefined_type *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
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
      this->emit_member_set (field, bt, " ::", "_ptr");
      this->emit_member_get (field, bt, " ::", "_ptr", "const");
      break;
    case AST_PredefinedType::PT_any:
      this->emit_member_set (field, bt, "const ::", " &");
      this->emit_member_get (field, bt, "const ::", " &", "const");
      this->emit_member_get (field, bt, "::", " &", "");
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      this->emit_member_set (field, bt, " ::", "");
      this->emit_member_get (field, bt, " ::", "", "const");
      break;
  }

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_sequence (be_sequence *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_sequence - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  this->emit_member_set (field, bt, "const ", " &");
  this->emit_member_get (field, bt, "const ", " &", "const");
  this->emit_member_get (field, bt, "", " &", "");

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_string (be_string *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_predefined_type - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  this->emit_member_set (field, bt, "", "");
  this->emit_member_set (field, bt, "const ", "");

  *os << "// Modifier to set the member." << be_nl;

  char const *string_type {""};
  if (node->node_type () == AST_Decl::NT_string)
    {
      string_type = "String";
    }
  else if (node->node_type () == AST_Decl::NT_wstring)
    {
      string_type = "WString";
    }

  *os << "ACE_INLINE void" << be_nl
      << vb_node_->name () << "::" << field->local_name ()
      << " (::CORBA::" << string_type << "_var const &val)"
      << be_nl << "{" << be_idt_nl
      << "this->_pd_value->" << field->local_name ()
      << " (val);" << be_uidt_nl
      << "}" << be_nl_2;

  this->emit_member_get (field, bt, "const ", "", "const");

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_structure (be_structure *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_structure - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  this->emit_member_set (field, bt, "const ", " &");
  this->emit_member_get (field, bt, "const ", " &", "const");
  this->emit_member_get (field, bt, "", " &", "");

  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (nullptr);
  return 0;
}

int
be_visitor_valuebox_union_member_ci::visit_union (be_union *node)
{
  be_decl *field = this->ctx_->node ();
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

  if (!field || !vb_node_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_union_member_ci::"
                         "visit_union - "
                         "bad context information\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  this->emit_member_set (field, bt, "const ", " &");
  this->emit_member_get (field, bt, "const ", " &", "const");
  this->emit_member_get (field, bt, "", " &", "");

  return 0;
}

void
be_visitor_valuebox_union_member_ci::emit_member_set (
  be_decl *field,
  be_type *field_type,
  char const *const_arg,
  char const *arg_modifier)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "// Modifier to set the member." << be_nl;

  *os << "ACE_INLINE void" << be_nl
      << vb_node_->name () << "::" << field->local_name () << " (";
  emit_union_member_type (os, const_arg, field_type->name (), arg_modifier);
  *os << " val)"
      << be_nl << "{" << be_idt_nl
      << "this->_pd_value->" << field->local_name ()
      << " (val);" << be_uidt_nl
      << "}" << be_nl_2;
}

void
be_visitor_valuebox_union_member_ci::emit_member_get (
  be_decl *field,
  be_type *field_type,
  char const *const_prefix,
  char const *type_suffix,
  char const *const_method)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "/// Accessor to retrieve the member." << be_nl;

  *os << "ACE_INLINE ";
  emit_union_member_type (os, const_prefix, field_type->name (), type_suffix);
  *os << be_nl
      << vb_node_->name () << "::" << field->local_name ()
      << " () " << const_method << be_nl
      << "{" << be_idt_nl
      << "return this->_pd_value->" << field->local_name ()
      << " ();" << be_uidt_nl
      << "}" << be_nl_2;
}
