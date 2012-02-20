
//=============================================================================
/**
 *  @file    field_cdr_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Field in the client stubs file.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> derived from be_visitor_field/cdr_op_cs.cpp
 */
//=============================================================================

#include "valuetype.h"

// **********************************************
//  Visitor for field in the client inline file.
// **********************************************

be_visitor_valuetype_field_cdr_cs::be_visitor_valuetype_field_cdr_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx),
    pre_ (""),
    post_ ("")
{
}

be_visitor_valuetype_field_cdr_cs::~be_visitor_valuetype_field_cdr_cs (void)
{
}

int
be_visitor_valuetype_field_cdr_cs::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_array - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // For anonymous arrays, the type name has a _ prepended. We compute
  // the full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscore
      // prepended to our local name. This needs to be inserted after
      // the parents's name.

      if (node->is_nested ())
        {
          be_decl *parent =
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (fname,
                           "%s::_%s",
                           parent->full_name (),
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname,
                           "_%s",
                           node->full_name ());
        }
    }
  else
    {
      // Typedefed node.
      ACE_OS::sprintf (fname,
                       "%s",
                       node->full_name ());
    }

  // Check what is the code generation substate. Are we generating
  // code for the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> "
          << "_tao_" << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << "
          << "_tao_" << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  // If not a typedef and we are defined in the use scope, we must be defined.
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // This is the case for anonymous arrays.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_array - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generation substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_enum - "
                         "bad sub state\n"),
                        -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_enum_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_interface - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ".out ())";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "TAO::Objref_Traits< ::" << node->name ()
          << ">::marshal (" << be_idt << be_idt_nl
          << this->pre_ << f->local_name ()
          << this->post_ << ".in ()," << be_nl
          << "strm" << be_uidt_nl
          << ")" << be_uidt;
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannit be declared inside a
      // structure.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_interface - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_interface_fwd - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ").out ()";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "TAO::Objref_Traits< ::" << node->name ()
          << ">::marshal (" << be_idt << be_idt_nl
          << this->pre_ << f->local_name ()
          << this->post_ << ".in ()," << be_nl
          << "strm" << be_uidt_nl
          << ")" << be_uidt;
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannot be declared inside a
      // structure.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_interface_fwd - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_valuebox (be_valuebox * node)
{
  return this->valuetype_common (node);
}

int
be_visitor_valuetype_field_cdr_cs::valuetype_common (be_type *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_valuetype - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ".out ())";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << this->pre_ << f->local_name ()
          << this->post_ << ".in ())";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because a valuetype cannit be declared inside a
      // structure.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_valuetype - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_valuetype (be_valuetype * node)
{
  return this->valuetype_common (node);
}

int
be_visitor_valuetype_field_cdr_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_valuetype_fwd - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ").out ()";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << this->pre_ << f->local_name ()
          << this->post_ << ").in ()";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because a valuetype cannot be declared inside a
      // structure.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_valuetype_fwd - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_valuetype_field_cdr_cs::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_valuetype_field_cdr_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_valuetype_field_cdr_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_valuetype_field_cdr_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_predefined_type - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  AST_PredefinedType::PredefinedType pt = node->pt ();

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          *os << "(strm >> " << this->pre_ << f->local_name ()
              << this->post_ << ".out ())";
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "(strm >> ::ACE_InputCDR::to_char ("
              << this->pre_ << f->local_name () << this->post_ << "))";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "(strm >> ::ACE_InputCDR::to_wchar ("
              << this->pre_ << f->local_name () << this->post_ << "))";
            }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "(strm >> ::ACE_InputCDR::to_octet ("
              << this->pre_ << f->local_name () << this->post_ << "))";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "(strm >> ::ACE_InputCDR::to_boolean ("
              << this->pre_ << f->local_name () << this->post_ << "))";
        }
      else
        {
          *os << "(strm >> " << this->pre_ << f->local_name ()
              << this->post_ << ")";
        }
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          *os << "(strm << " << this->pre_ << f->local_name ()
              << this->post_ << ".in ())";
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "(strm << ::ACE_OutputCDR::from_char ("
              << this->pre_ << f->local_name () << this->post_ << "))";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "(strm << ::ACE_OutputCDR::from_wchar ("
              << this->pre_ << f->local_name () << this->post_ << "))";
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "(strm << ::ACE_OutputCDR::from_octet ("
              << this->pre_ << f->local_name () << this->post_ << "))";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "(strm << ::ACE_OutputCDR::from_boolean ("
              << this->pre_ << f->local_name () << this->post_ << "))";
        }
      else
        {
          *os << "(strm << " << this->pre_ << f->local_name ()
              << this->post_ << ")";
        }
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_sequence - "
                         "cannot retrieve field node\n" ),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_sequence - "
                         "bad sub state\n"),
                        -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    // Not a typedef AND
    // node is defined inside the valuetype.
    {
      // Anonymous sequence.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_string (be_string *str)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_string - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      if (str != 0 && str->max_size ()->ev ()->u.ulval != 0)
        {
          if (str->width () == (long) sizeof (char))
            {
              *os << "(strm >> ACE_InputCDR::to_string ("
                  << this->pre_ << f->local_name () << this->post_
                  << ".out (), " << str->max_size ()->ev ()->u.ulval << "))";
            }
          else
            {
              *os << "(strm >> ACE_InputCDR::to_wstring ("
                  << this->pre_ << f->local_name () << this->post_
                  << ".out (), " << str->max_size ()->ev ()->u.ulval << "))";
            }
        }
      else
        {
          *os << "(strm >> " << this->pre_ << f->local_name ()
              << this->post_ << ".out ())";
        }
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      if (str != 0 && str->max_size ()->ev ()->u.ulval != 0)
        {
          if (str->width () == (long) sizeof (char))
            {
              *os << "(strm << ACE_OutputCDR::from_string ("
                  <<  this->pre_ << f->local_name () << this->post_
                  << ".in (), " << str->max_size ()->ev ()->u.ulval << "))";
            }
          else
            {
              *os << "(strm << ACE_OutputCDR::from_wstring ("
                  <<  this->pre_ << f->local_name () << this->post_
                  << ".in (), " << str->max_size ()->ev ()->u.ulval << "))";
            }
        }
      else
        {
          *os << "(strm << " << this->pre_ << f->local_name ()
              << this->post_ << ".in ())";
        }
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_structure (
  be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_structure - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_structure - "
                         "bad sub state\n"),
                        -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                             "visit_struct - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_valuetype_field_cdr_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_union - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << this->pre_ << f->local_name ()
          << this->post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                         "visit_union - "
                         "bad sub state\n"),
                        -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_cs::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}


// ****************************************************************

be_visitor_valuetype_field_cdr_decl::be_visitor_valuetype_field_cdr_decl (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

// This is a field, but the action depends on the type of the field,
// use this visitor to detect the type of the field.
// Notice that this is why the parent visitor (who create us) cannot
// do the job, because it may have another purpose for some or all of
// the visit_* methods; in other words, while running a visitor to
// generate CDR operators for structures we cannot use that one to
// generate the code of each field, because visit_struct already has a
// meaning in that visitor.
int
be_visitor_valuetype_field_cdr_decl::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // @@ Shouldn't this be saved in the visitor and not the context?!
  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_decl::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_array - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Retrieve the valuetype scope in which the code is generated.
  be_decl *sc = this->ctx_->scope ()->decl ();
  be_valuetype *vt = be_valuetype::narrow_from_decl (sc);

  if (!vt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_array - "
                         "cannot retrieve valuetype node\n"),
                        -1);
    }

  // For anonymous arrays, the type name has a _ prepended. We compute
  // the full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope
      // prepended to our local name. This needs to be inserted after
      // the parent's name.

      if (node->is_nested ())
        {
          be_decl *parent =
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (fname,
                           "%s::_%s",
                           parent->full_name (),
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname,
                           "_%s",
                           node->full_name ());
        }
    }
  else
    {
      // Typedefed node.
      ACE_OS::sprintf (fname,
                       "%s",
                       node->full_name ());
    }

  // Check what is the code generation substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << fname << "_forany "
          << "_tao_" << vt->field_pd_prefix () << f->local_name ()
          << vt->field_pd_postfix () << be_idt << be_idt_nl
          << "(const_cast<" << be_idt << be_idt_nl
          << fname << "_slice*> (" << be_nl
          << vt->field_pd_prefix () << f->local_name ()
          << vt->field_pd_postfix () << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl;
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_array - "),
                        -1);
    }
}

int
be_visitor_valuetype_field_cdr_decl::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_cdr_op_field_decl::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}
