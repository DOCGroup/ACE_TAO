
//=============================================================================
/**
 *  @file    exception_ctor.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the special constructor for Exceptions
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "exception.h"

be_visitor_exception_ctor::be_visitor_exception_ctor (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_exception_ctor::~be_visitor_exception_ctor (void)
{
}

int
be_visitor_exception_ctor::post_process (be_decl *bd)
{
  // This checks for members of an enum 'non-scope' declared inside
  // the exception. If an enum val is actually a member, it will
  // have node type NT_field.
  if (bd->node_type () == AST_Decl::NT_enum_val)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (!this->last_node (bd))
    {
      *os << "," << be_nl;
    }

  return 0;
}

int
be_visitor_exception_ctor::visit_exception (be_exception *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << node->local_name ();
    }
  else
    {
      *os << node->name () << "::" << node->local_name ();
    }

  *os << " (" << be_idt << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor_assign::"
                         "visit_exception - "
                         "codegen for scope failed\n"),
                        -1);
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << ");" << be_uidt << be_uidt;
    }
  else
    {
      *os << ")" << be_uidt << be_uidt;
    }

  return 0;
}

int
be_visitor_exception_ctor::visit_field (be_field *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // Retrieve the type.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor::"
                         "visit_exception - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor::"
                         "visit_field - "
                         "cannot accept visitor\n"),
                        -1);
    }

  *os << " _tao_" << node->local_name ();

  return 0;
}

int
be_visitor_exception_ctor::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << "const ";

  be_typedef *tdef = be_typedef::narrow_from_decl (bt);

  if (this->ctx_->state () != TAO_CodeGen::TAO_EXCEPTION_CTOR_CH
      && !tdef)
    {
      *os << this->ctx_->scope ()->decl ()->full_name () << "::";
    }

  if (!tdef)
    {
      *os << "_";
    }

  *os << bt->nested_type_name (this->ctx_->scope ()->decl ());

  return 0;
}

int
be_visitor_exception_ctor::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << bt->nested_type_name (this->ctx_->scope ()->decl ());
    }
  else
    {
      *os << bt->name ();
    }

  return 0;
}

int
be_visitor_exception_ctor::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << "const " << bt->nested_type_name (this->ctx_->scope ()->decl (), "_ptr");
    }
  else
    {
      *os << "const " << bt->name () << "_ptr";
    }

  return 0;
}

int
be_visitor_exception_ctor::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << "const " << bt->nested_type_name (this->ctx_->scope ()->decl (), "_ptr");
    }
  else
    {
      *os << "const " << bt->name () << "_ptr";
    }

  return 0;
}

int
be_visitor_exception_ctor::visit_valuebox (be_valuebox *node)
{
  return this->emit_common (node);
}

int
be_visitor_exception_ctor::visit_valuetype (be_valuetype *node)
{
  return this->emit_common (node);
}

int
be_visitor_exception_ctor::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->emit_common (node);
}

int
be_visitor_exception_ctor::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;
  be_typedef *td = this->ctx_->alias ();

  if (td != 0)
    {
      bt = td;
    }
  else
    {
      bt = node;
    }

  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_any)
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
        {
          *os << "const "
              << bt->nested_type_name (this->ctx_->scope ()->decl ());
        }
      else
        {
          *os << "const ::" << bt->name ();
        }

      *os << " &";

      return 0;
    }
  else if (pt == AST_PredefinedType::PT_pseudo
           || pt == AST_PredefinedType::PT_object)
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
        {
          *os << "const "
              << bt->nested_type_name (this->ctx_->scope ()->decl (), "_ptr ");
        }
      else
        {
          *os << "const ::" << bt->name () << "_ptr ";
        }
    }
  else
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
        {
          *os << bt->nested_type_name (this->ctx_->scope ()->decl ());
        }
      else
        {
          *os << "::" << bt->name ();
        }
    }

  return 0;
}

int
be_visitor_exception_ctor::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << "const " << bt->nested_type_name (this->ctx_->scope ()->decl ());
    }
  else
    {
      *os << "const " << bt->name ();
    }

  *os << " &";

  return 0;
}

int
be_visitor_exception_ctor::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (node->width () == (long) sizeof (char))
    {
      *os << "const char *";
    }
  else
    {
      *os << "const ::CORBA::WChar *";
    }

  return 0;
}

int
be_visitor_exception_ctor::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << "const " << bt->nested_type_name (this->ctx_->scope ()->decl ());
    }
  else
    {
      *os << "const " << bt->name ();
    }

  *os << " &";

  return 0;
}

int
be_visitor_exception_ctor::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_exception_ctor::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << "const " << bt->nested_type_name (this->ctx_->scope ()->decl ());
    }
  else
    {
      *os << "const " << bt->name ();
    }

  *os << " &";

  return 0;
}

int
be_visitor_exception_ctor::visit_union_fwd (be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

int
be_visitor_exception_ctor::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_exception_ctor::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_exception_ctor::emit_common (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_EXCEPTION_CTOR_CH)
    {
      *os << "const " << bt->nested_type_name (this->ctx_->scope ()->decl (), " *");
    }
  else
    {
      *os << "const " << bt->name () << " *";
    }

  return 0;
}
