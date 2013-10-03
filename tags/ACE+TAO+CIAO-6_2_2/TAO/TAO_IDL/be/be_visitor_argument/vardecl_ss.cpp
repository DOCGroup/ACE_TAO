
//=============================================================================
/**
 *  @file    vardecl_ss.cpp
 *
 *  $Id$
 *
 *  Visitor that generates the variable declaration in the skeleton
 *  corresponding to the Argument node
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "argument.h"

// ************************************************************************
// Visitor to generate code for argument variable declaration
// ************************************************************************

be_visitor_args_vardecl_ss::be_visitor_args_vardecl_ss (
    be_visitor_context *ctx
  )
  : be_visitor_args (ctx)
{
}

be_visitor_args_vardecl_ss::~be_visitor_args_vardecl_ss (void)
{
}

int be_visitor_args_vardecl_ss::visit_argument (
  be_argument *node)
{
  this->ctx_->node (node);
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_vardecl_ss::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("Bad argument type\n")),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl;

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_vardecl_ss::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("cannot accept visitor\n")),
                        -1);
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_array (
  be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "::" << bt->name () << " "
          << arg->local_name () << ";" << be_nl
          << "::" << bt->name () << "_forany _tao_forany_"
          << arg->local_name () << " ("
          << be_idt << be_idt_nl
          << arg->local_name () << be_uidt_nl
          << ");" << be_uidt;

      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << "::" << bt->name ()
              << "_var " << arg->local_name ()
              << ";";
        }
      else
        {
          *os << bt->name () << " " << arg->local_name ()
              << ";";
        }

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "::" << bt->name ()
          << " " << arg->local_name () << ";";

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_interface (
  be_interface *node)
{
  return this->emit_common (node);
}

int be_visitor_args_vardecl_ss::visit_interface_fwd (
  be_interface_fwd *node)
{
  return this->emit_common (node);
}


int be_visitor_args_vardecl_ss::visit_valuebox (
  be_valuebox *node)
{
  return this->emit_common (node);
}

int be_visitor_args_vardecl_ss::visit_valuetype (
  be_valuetype *node)
{
  return this->emit_common (node);
}

int be_visitor_args_vardecl_ss::visit_valuetype_fwd (
  be_valuetype_fwd *node)
{
  return this->emit_common (node);
}

int be_visitor_args_vardecl_ss::visit_predefined_type (
  be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  AST_PredefinedType::PredefinedType pt = node->pt ();

  *os << "::" << bt->name ();

  if (pt == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << " ";
          break;
        case AST_Argument::dir_OUT:
          *os << "_var ";
          break;
        }

      *os << arg->local_name () << ";";
    }
  else if (pt == AST_PredefinedType::PT_pseudo
           || pt == AST_PredefinedType::PT_object)
    {
      *os << "_var " << arg->local_name () << ";";
    }
  else
    {
      *os << " " << arg->local_name ();

      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;

        // @@@ (JP) This is a hack for VC7, which gets an internal
        // compiler error if these not initialized.
        // (02-12-09)
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          switch (pt)
            {
              case AST_PredefinedType::PT_boolean:
              case AST_PredefinedType::PT_short:
              case AST_PredefinedType::PT_long:
              case AST_PredefinedType::PT_ushort:
              case AST_PredefinedType::PT_ulong:
              case AST_PredefinedType::PT_ulonglong:
              case AST_PredefinedType::PT_float:
              case AST_PredefinedType::PT_double:
              case AST_PredefinedType::PT_octet:
                *os << " = 0";
                break;
              case AST_PredefinedType::PT_longlong:
                *os << " = ACE_CDR_LONGLONG_INITIALIZER";
                break;
              case AST_PredefinedType::PT_longdouble:
                *os << " = ACE_CDR_LONG_DOUBLE_INITIALIZER";
                break;
              default:
                break;
            }

          break;
        }

      *os << ";";
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_sequence (
  be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "::" << bt->name () << " "
          << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      *os << "::" << bt->name () << "_var "
          << arg->local_name () << ";" << be_nl;

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_string (
  be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      if (node->width () == (long) sizeof (char))
        {
          *os << "::CORBA::String_var "
              << arg->local_name () << ";";
        }
      else
        {
          *os << "::CORBA::WString_var "
              << arg->local_name () << ";";
        }

      break;
    case AST_Argument::dir_OUT:
      if (node->width () == (long )sizeof (char))
        {
          *os << "::CORBA::String_var "
              << arg->local_name () << ";";
        }
      else
        {
          *os << "::CORBA::WString_var "
              << arg->local_name () << ";";
        }

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_structure (
  be_structure *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_vardecl_ss::visit_union (
  be_union *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_vardecl_ss::visit_typedef (
  be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_vardecl_ss::")
                         ACE_TEXT ("visit_typedef - ")
                         ACE_TEXT ("accept on primitive ")
                         ACE_TEXT ("type failed\n")),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_args_vardecl_ss::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

int
be_visitor_args_vardecl_ss::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_args_vardecl_ss::visit_eventtype (
    be_eventtype *node
  )
{
  return this->visit_valuetype (node);
}

int
be_visitor_args_vardecl_ss::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}


int
be_visitor_args_vardecl_ss::emit_common (
  be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "::" << bt->name () << "_var "
          << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      *os << "::" << bt->name () << "_var "
          << arg->local_name () << ";";

      break;
    }

  return 0;
}



int
be_visitor_args_vardecl_ss::emit_common2 (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "::" << bt->name () << " "
          << arg->local_name () << ";";
      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << "::" << bt->name () << "_var "
              << arg->local_name () << ";";
        }
      else
        {
          *os << "::" << bt->name () << " "
              << arg->local_name () << ";";
        }

      break;
    }

  return 0;
}
