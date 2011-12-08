
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Field in the client stubs file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "be_visitor_array/cdr_op_cs.h"
#include "be_visitor_sequence/cdr_op_cs.h"
#include "be_visitor_structure/cdr_op_cs.h"
#include "be_visitor_union/cdr_op_cs.h"

// **********************************************
//  visitor for field in the client stubs file
// **********************************************

be_visitor_field_cdr_op_cs::be_visitor_field_cdr_op_cs (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_field_cdr_op_cs::~be_visitor_field_cdr_op_cs (void)
{
}

int
be_visitor_field_cdr_op_cs::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_array (be_array *node)
{
  // If the array is defined in this scope, we must generate
  // CDR stream operators for the array itself.
  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_cs::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Now generate the (de)marshaling code for the array as a
  // field.

  TAO_OutStream *os = this->ctx_->stream ();

  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_array - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // For anonymous arrays, the type name has a _ prepended. We compute
  // the full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];

  // Save the node's local name and full name in a buffer for quick
  // use later on.
  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  if (this->ctx_->alias () == 0 // Not a typedef.
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // For anonymous arrays ...
      // we have to generate a name for us that has an underscore
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
          << "_tao_aggregate_" << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << "
          << "_tao_aggregate_" << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // This is done in cdr_op_cs and hacked into *.i.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_enum (be_enum *node)
{
  // If we are defined inside this scope, we must generate the
  /// CDR stream operators for us here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_enum_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_cs::"
                             "visit_enum - "
                             "codegen failed\n"),
                            -1);
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_enum - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_enum - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_interface - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ".out ())";

      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      if (node->is_defined ())
        {
          if (f->is_abstract ())
            {
              *os << "(strm << _tao_aggregate."
                  << f->local_name () << ".in ())";
            }
          else
            {
              *os << "::CORBA::Object::marshal (" << be_idt << be_idt_nl
                  << "_tao_aggregate." << f->local_name () << ".in ()," << be_nl
                  << "strm" << be_uidt_nl
                  << ")" << be_uidt;
            }
        }
      else
        {
          *os << "TAO::Objref_Traits<" << node->name () << ">::marshal ("
              << be_idt << be_idt_nl
              << "_tao_aggregate." << f->local_name () << ".in ()," << be_nl
              << "strm" << be_uidt_nl
              << ")" << be_uidt;
        }

      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannot be declared inside a
      // structure.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_interface - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_field_cdr_op_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_interface_fwd - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ".out ())";

      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      if (node->is_defined ())
        {
          if (f->is_abstract ())
            {
              *os << "(strm << _tao_aggregate."
                  << f->local_name () << ".in ())";
            }
          else
            {
              *os << "::CORBA::Object::marshal (" << be_idt << be_idt_nl
                  << "_tao_aggregate." << f->local_name () << ".in ()," << be_nl
                  << "strm" << be_uidt_nl
                  << ")" << be_uidt;
            }
        }
      else
        {
          *os << "TAO::Objref_Traits<" << node->name () << ">::marshal ("
              << be_idt << be_idt_nl
              << "_tao_aggregate." << f->local_name () << ".in ()," << be_nl
              << "strm" << be_uidt_nl
              << ")" << be_uidt;
        }

      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because an interface cannot be declared inside a
      // structure.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_interface_fwd - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_field_cdr_op_cs::visit_valuebox (be_valuebox *)
{
  return this->emit_valuetype_common ();
}

int
be_visitor_field_cdr_op_cs::visit_valuetype (be_valuetype *)
{
  return this->emit_valuetype_common ();
}

int
be_visitor_field_cdr_op_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_field_cdr_op_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  return this->emit_valuetype_common ();
}

int
be_visitor_field_cdr_op_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}


int
be_visitor_field_cdr_op_cs::emit_valuetype_common (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "emit_common - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ".out ())";

      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_aggregate." << f->local_name () << ".in ())";

      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done because a valuetype cannot be declared inside a
      // structure.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "emit_common - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}


// Visit predefined type.
int
be_visitor_field_cdr_op_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
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
          *os << "(strm >> _tao_aggregate." << f->local_name ()
              << ".out ())";
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "(strm >> ::ACE_InputCDR::to_char (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "(strm >> ::ACE_InputCDR::to_wchar (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "(strm >> ::ACE_InputCDR::to_octet (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "(strm >> ::ACE_InputCDR::to_boolean (_tao_aggregate."
              << f->local_name () << "))";
        }
      else
        {
          *os << "(strm >> _tao_aggregate." << f->local_name () << ")";
        }
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          *os << "(strm << _tao_aggregate." << f->local_name () << ".in ())";
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "(strm << ::ACE_OutputCDR::from_char (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "(strm << ::ACE_OutputCDR::from_wchar (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "(strm << ::ACE_OutputCDR::from_octet (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "(strm << ::ACE_OutputCDR::from_boolean (_tao_aggregate."
              << f->local_name () << "))";
        }
      else
        {
          *os << "(strm << _tao_aggregate." << f->local_name () << ")";
        }
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_sequence (be_sequence *node)
{
  // If the sequence is defined in this scope, generate its
  // CDR stream operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_cs::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // How generate the marshaling code for the sequence as a field.

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_sequence - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_sequence - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_string (be_string *str)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_string - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // check what is the code generation's substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      if (str != 0 && str->max_size ()->ev ()->u.ulval != 0)
        {
          if (str->width () == (long) sizeof (char))
            {
              *os << "(strm >> ACE_InputCDR::to_string (_tao_aggregate."
                  << f->local_name () << ".out (), "
                  << str->max_size ()->ev ()->u.ulval << "))";
            }
          else
            {
              *os << "(strm >> ACE_InputCDR::to_wstring (_tao_aggregate."
                  << f->local_name () << ".out (), "
                  << str->max_size ()->ev ()->u.ulval << "))";
            }
        }
      else
        {
          *os << "(strm >> _tao_aggregate." << f->local_name () << ".out ())";
        }

      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      if (str != 0 && str->max_size ()->ev ()->u.ulval != 0)
        {
          if (str->width () == (long) sizeof (char))
            {
              *os << "(strm << ACE_OutputCDR::from_string (_tao_aggregate."
                  << f->local_name () << ".in (), "
                  << str->max_size ()->ev ()->u.ulval << "))";
            }
          else
            {
              *os << "(strm << ACE_OutputCDR::from_wstring (_tao_aggregate."
                  << f->local_name () << ".in (), "
                  << str->max_size ()->ev ()->u.ulval << "))";
            }
        }
      else
        {
          *os << "(strm << _tao_aggregate." << f->local_name () << ".in ())";
        }

      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_array - "
                         "bad substate\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_structure (be_structure *node)
{
  // If the struct is defined in this scope, generate its CDR stream
  // operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_cs::"
                             "visit_struct - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // How generate the marshaling code for the struct as a field.

  TAO_OutStream *os = this->ctx_->stream ();

  // retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_structure - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_structure - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    be_structure::narrow_from_decl (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_field_cdr_op_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_union (be_union *node)
{
  // If the union is defined in this scope, generate its CDR stream
  // operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_cdr_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_cs::"
                             "visit_union - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Now generate marshaling code for the union as a field.

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_union - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_aggregate." << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Proceed further.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_cs::"
                         "visit_union - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_cdr_op_cs::visit_union_fwd (be_union_fwd *node)
{
  be_union *u =
    be_union::narrow_from_decl (node->full_definition ());

  return this->visit_union (u);
}

// ****************************************************************

be_visitor_cdr_op_field_decl::be_visitor_cdr_op_field_decl (
      be_visitor_context *ctx)
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
be_visitor_cdr_op_field_decl::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_cdr_op_field_decl::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // @@ Shouldn't this be saved in the visitor and not the context?!
  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_cdr_op_field_decl::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_cdr_op_field_decl::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_cdr_op_field_decl::"
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

  if (this->ctx_->alias () == 0 // Not a typedef.
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // For anonymous arrays,
      // we have to generate a name for us that has an underscope
      // prepended to our local name. This needs to be inserted after
      // the parents's name.

      if (node->is_nested ())
        {
          be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
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
          << "_tao_aggregate_" << f->local_name () << be_idt << be_idt_nl
          << "#if defined __IBMCPP__ && __IBMCPP__ <= 800" << be_nl
          << "((" << fname << "_slice*) (" << be_nl
          << "#else" << be_nl
          << "(const_cast<" << be_idt << be_idt_nl
          << fname << "_slice*> (" << be_uidt << be_uidt_nl
          << "#endif" << be_idt << be_idt_nl
          << "_tao_aggregate." << f->local_name () << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl;

      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_cdr_op_field_decl::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}


int
be_visitor_cdr_op_field_decl::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // Save the typedef node for use in code
                            // generation as we visit the base type.

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (bt == 0 || bt->accept (this) == -1)
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
