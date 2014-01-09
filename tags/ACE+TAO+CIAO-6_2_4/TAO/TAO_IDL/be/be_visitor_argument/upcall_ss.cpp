
//=============================================================================
/**
 *  @file    upcall_ss.cpp
 *
 *  $Id$
 *
 *  Visitor that generates code that passes argument variables to the
 *  upcall.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "argument.h"

// ************************************************************************
// visitor for passing arguments to the upcall
// ************************************************************************

be_visitor_args_upcall_ss::be_visitor_args_upcall_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_upcall_ss::~be_visitor_args_upcall_ss (void)
{
}

int be_visitor_args_upcall_ss::visit_argument (be_argument *node)
{
  this->ctx_->node (node);
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_upcall_ss::")
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
                         ACE_TEXT ("be_visitor_args_upcall_ss::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("cannot accept visitor\n")),
                        -1);
    }

  return 0;
}

int be_visitor_args_upcall_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      // This is to placate some compilers which have
      // trouble with IN args that are multidimensional arrays.
      if (node->n_dims () > 1)
        {
          *os << "(const ::" << node->name () << "_slice *) ";
        }

      *os << arg->local_name ();

      break;
    case AST_Argument::dir_INOUT:
      *os << arg->local_name ();

      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == AST_Type::VARIABLE)
        {
          if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
            {
              *os << arg->local_name ();
            }
          else
            {
              *os << arg->local_name () << ".out ()";
            }
        }
      else
        {
          *os << arg->local_name ();
        }

      break;
    }

  return 0;
}

int be_visitor_args_upcall_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << arg->local_name ();

      break;
    }

  return 0;
}

int be_visitor_args_upcall_ss::visit_interface (
  be_interface *)
{
  return this->emit_common ();
}

int be_visitor_args_upcall_ss::visit_interface_fwd (
  be_interface_fwd *)
{
  return this->emit_common ();
}

int be_visitor_args_upcall_ss::visit_valuebox (
  be_valuebox *)
{
  return this->emit_common ();
}

int be_visitor_args_upcall_ss::visit_valuetype (
  be_valuetype *)
{
  return this->emit_common ();
}

int be_visitor_args_upcall_ss::visit_valuetype_fwd (
  be_valuetype_fwd *)
{
  return this->emit_common ();
}

int be_visitor_args_upcall_ss::visit_predefined_type (
  be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();

          break;
        case AST_Argument::dir_OUT:
          if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
            {
              *os << arg->local_name ();
            }
          else
            {
              *os << arg->local_name () << ".out ()";
            }

          break;
        }
    }
  else if (pt == AST_PredefinedType::PT_pseudo
           || pt == AST_PredefinedType::PT_object)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
            {
              *os << arg->local_name ();
            }
          else
            {
              *os << arg->local_name () << ".in ()";
            }

          break;
        case AST_Argument::dir_INOUT:
          if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
            {
              *os << arg->local_name ();
            }
          else
            {
              *os << arg->local_name () << ".inout ()";
            }

          break;
        case AST_Argument::dir_OUT:
          if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
            {
              *os << arg->local_name ();
            }
          else
            {
              *os << arg->local_name () << ".out ()";
            }

          break;
        }
    }
  else
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();

          break;
        }
    }

  return 0;
}

int be_visitor_args_upcall_ss::visit_sequence (
  be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << arg->local_name ();

      break;
    case AST_Argument::dir_OUT:
      if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
        {
          *os << arg->local_name ();
        }
      else
        {
          *os << arg->local_name () << ".out ()";
        }

      break;
    }

  return 0;
}

int be_visitor_args_upcall_ss::visit_string (
  be_string *)
{
  return this->emit_common ();
}

int be_visitor_args_upcall_ss::visit_structure (
  be_structure *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_upcall_ss::visit_union (
  be_union *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_upcall_ss::visit_typedef (
  be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_upcall_ss::")
                         ACE_TEXT ("visit_typedef - ")
                         ACE_TEXT ("accept on primitive ")
                         ACE_TEXT ("type failed\n")),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_args_upcall_ss::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

int
be_visitor_args_upcall_ss::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_args_upcall_ss::visit_eventtype (
    be_eventtype *node
  )
{
  return this->visit_valuetype (node);
}

int
be_visitor_args_upcall_ss::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}

int be_visitor_args_upcall_ss::emit_common (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      if (this->ctx_->state ()
            == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
        {
          *os << arg->local_name ();
        }
      else
        {
          *os << arg->local_name () << ".in ()";
        }

      break;
    case AST_Argument::dir_INOUT:
      if (this->ctx_->state ()
            == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
        {
          *os << arg->local_name ();
        }
      else
        {
          *os << arg->local_name () << ".inout ()";
        }

      break;
    case AST_Argument::dir_OUT:
      if (this->ctx_->state ()
            == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
        {
          *os << arg->local_name ();
        }
      else
        {
          *os << arg->local_name () << ".out ()";
        }

      break;
    }

  return 0;
}

int be_visitor_args_upcall_ss::emit_common2 (
  be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << arg->local_name ();

      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == AST_Type::VARIABLE)
        {
          if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS)
            {
              *os << arg->local_name ();
            }
          else
            {
              *os << arg->local_name () << ".out ()";
            }
        }
      else
        {
          *os << arg->local_name ();
        }

      break;
    }

  return 0;
}
