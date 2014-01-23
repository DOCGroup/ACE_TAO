
//=============================================================================
/**
 *  @file    arglist.cpp
 *
 *  $Id$
 *
 *  Visitor that generates the parameters in an Operation signature
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "argument.h"

// ************************************************************
// be_visitor_args_arglist for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_arglist::be_visitor_args_arglist (be_visitor_context *ctx)
  : be_visitor_args (ctx),
    unused_ (false)
{
}

be_visitor_args_arglist::~be_visitor_args_arglist (void)
{
}

int be_visitor_args_arglist::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Retrieve the type.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type.
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  if (this->ctx_->state () != TAO_CodeGen::TAO_TIE_OPERATION_ARGLIST_SH)
    {
      *os << " " << (unused_ ? "/* " : "" )
          << node->local_name ()->get_string ()
          << (unused_ ? " */" : "");
    }

  return 0;
}

int be_visitor_args_arglist::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr") << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr") << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int be_visitor_args_arglist::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int be_visitor_args_arglist::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node) << " &";
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const " << this->type_name (node) << " &";
          break;
        case AST_Argument::dir_INOUT:
          *os << this->type_name (node) << " &";
          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        }
    }
  else if (pt == AST_PredefinedType::PT_value)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << this->type_name (node) << " *";
          break;
        case AST_Argument::dir_INOUT:
          *os << this->type_name (node) << " *&";
          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        }
    }
  else if (pt == AST_PredefinedType::PT_pseudo
           || pt == AST_PredefinedType::PT_object
           || pt == AST_PredefinedType::PT_abstract)
    {
      // The only PT_pseudo that doesn't take a _ptr suffix.
      bool is_tckind =
        (ACE_OS::strcmp (node->local_name ()->get_string (), "TCKind") == 0);

      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          if (is_tckind)
            {
              *os << this->type_name (node);
            }
          else
            {
              *os << this->type_name (node, "_ptr");
            }

          break;
        case AST_Argument::dir_INOUT:
          if (is_tckind)
            {
              *os << this->type_name (node) << " &";
            }
          else
            {
              *os << this->type_name (node, "_ptr") << " &";
            }

          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        }
    }
  else
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << this->type_name (node);
          break;
        case AST_Argument::dir_INOUT:
          *os << this->type_name (node) << " &";
          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        }
    }

  return 0;
}

int be_visitor_args_arglist::visit_sequence (be_sequence *node)
{
  // There seems to be one case where the two conditions below
  // are true - in generating get/set operations for an
  // inherited valuetype member, which is included from
  // another IDL file, and whose type is an anonymous
  // sequence. There is also no better place to make the
  // call to create_name() - the node constructor sets the
  // 'anonymous' flag to false, the typedef that resets it
  // to true is created afterward. And any member of an
  // included IDL declaration is not processed as part of
  // the AST traversal. If create_name() is never called,
  // then 'type_name' below will output 'sequence'.
  if (node->imported () && node->anonymous ())
    {
      (void) node->create_name (0);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      if (be_global->alt_mapping () && node->unbounded ())
        {
          *os << this->type_name (node) << " &";
        }
      else
        {
          *os << this->type_name (node, "_out");
        }

      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  ACE_CDR::ULong bound = node->max_size ()->ev ()->u.ulval;
  bool wide = (node->width () != (long) sizeof (char));

  if (!wide && bound == 0 && be_global->alt_mapping ())
    {
      switch (this->direction ())
        {
          case AST_Argument::dir_IN:
            *os << "const std::string";
            break;
          default:
            *os << "std::string &";
            break;
        }

      return 0;
    }

  if (node->width () == (long) sizeof (char))
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const char *";
          break;
        case AST_Argument::dir_INOUT:
          *os << "char *&";
          break;
        case AST_Argument::dir_OUT:
          *os << "::CORBA::String_out";
          break;
        }
    }
  else
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const ::CORBA::WChar *";
          break;
        case AST_Argument::dir_INOUT:
          *os << "::CORBA::WChar *&";
          break;
        case AST_Argument::dir_OUT:
          *os << "::CORBA::WString_out";
          break;
        }
    }

  return 0;
}

int be_visitor_args_arglist::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}


int be_visitor_args_arglist::visit_valuetype (be_valuetype *node)
{
  return this->emit_common (node);
}

int be_visitor_args_arglist::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node) << " *";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " *&";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int be_visitor_args_arglist::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_args_arglist::visit_home (
    be_home *node
  )
{
  return this->visit_interface (node);
}

int be_visitor_args_arglist::visit_valuebox (be_valuebox *node)
{
  return this->emit_common (node);
}


int be_visitor_args_arglist::emit_common (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node) << " *";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " *&";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

void
be_visitor_args_arglist::unused (bool val)
{
  this->unused_ = val;
}
