
//=============================================================================
/**
 *  @file    serializer_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating TAO::DCPS::Serializer operators code for Field in
 *  the client stubs file.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#include "be_visitor_array/serializer_op_cs.h"
#include "be_visitor_sequence/serializer_op_cs.h"
#include "be_visitor_structure/serializer_op_cs.h"
#include "be_visitor_union/serializer_op_cs.h"

// **********************************************
//  visitor for field in the client stubs file
// **********************************************

be_visitor_field_serializer_op_cs::be_visitor_field_serializer_op_cs (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_field_serializer_op_cs::~be_visitor_field_serializer_op_cs (void)
{
}

int
be_visitor_field_serializer_op_cs::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_array (be_array *node)
{
  // If the array is defined in this scope, we must generate
  // CDR stream operators for the array itself.
  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_array_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_cs::"
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
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
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      *os << "_tao_is_bounded_size (_tao_aggregate_" << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      // get the size of an array typedef
      *os << "_dcps_max_marshaled_size (_tao_aggregate_" << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      // get the size of an array typedef
      *os << "_dcps_find_size (_tao_aggregate_" << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> "
          << "_tao_aggregate_" << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << "
          << "_tao_aggregate_" << f->local_name () << ")";

      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // This is done in serializer_op_cs and hacked into *.i.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_enum (be_enum *node)
{
  // If we are defined inside this scope, we must generate the
  /// Serializer stream operators for us here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_enum_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_cs::"
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_enum - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      *os << " true /* enum */";
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      // enums are serialized as CORBA::ULong
      *os << "_dcps_max_marshaled_size_ulong () /* enum */";
      break;
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_enum - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_interface - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_field_serializer_op_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
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
          AST_Decl *parent = ScopeAsDecl (node->defined_in ());

          if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
            {
              *os << parent->name () << "::";
            }

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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_interface_fwd - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_field_serializer_op_cs::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_valuetype - "
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_valuetype - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_field_serializer_op_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_valuetype_fwd - "
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_valuetype_fwd - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

// Visit predefined type.
int
be_visitor_field_serializer_op_cs::visit_predefined_type (
  be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_predefined_type - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  AST_PredefinedType::PredefinedType pt = node->pt ();

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {

  case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_predefined_type - TAO_IS_BOUNDED_SIZE  "
                         "object and psuedo are not supported by DDS\n"),
                        -1);
        }

      else
        {
          *os << " true "; // all predefined types are bounded
        }
      break;
  case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
  case TAO_CodeGen::TAO_FIND_SIZE: // the max and current sizes are the same for predefined types
      if (pt == AST_PredefinedType::PT_pseudo
          || pt == AST_PredefinedType::PT_object)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_predefined_type - TAO_MAX_MARSHALED_SIZE  "
                         "object and psuedo are not supported by DDS\n"),
                        -1);
        }
      else if (pt == AST_PredefinedType::PT_char)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_char (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_wchar)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_wchar (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_octet)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_octet (_tao_aggregate."
              << f->local_name () << "))";
        }
      else if (pt == AST_PredefinedType::PT_boolean)
        {
          *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_boolean (_tao_aggregate."
              << f->local_name () << "))";
        }
      else
        {
          *os << "_dcps_max_marshaled_size (_tao_aggregate." << f->local_name () << ")";
        }
      break;
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_predefined_type - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_sequence (be_sequence *node)
{
  // If the sequence is defined in this scope, generate its
  // Serializer stream operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_sequence_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_cs::"
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_sequence - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      *os << "_tao_is_bounded_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      *os << "_dcps_max_marshaled_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << "_dcps_find_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_sequence - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_string - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // check what is the code generation's substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
       if (0 == node->max_size ()->ev ()->u.ulval)
         *os << "false /* unbounded string */";
       else
         *os << "true /* bounded string */";
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      char buff[15];
      ACE_OS::sprintf(buff, "%ld", node->max_size ()->ev ()->u.ulval);
      *os << "_dcps_max_marshaled_size_ulong () + " << buff;
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      if (node->node_type() == AST_Decl::NT_wstring)
      {
        *os << "_dcps_max_marshaled_size_ulong() + "
          << "(_tao_aggregate." << f->local_name () << ".in () ? "
            << "ACE_OS::strlen(_tao_aggregate."
            << f->local_name () << ".in ()) * sizeof (CORBA::WChar) : 0)";
      }
      else
      {
        *os << "_dcps_max_marshaled_size_ulong() + "
            << "(_tao_aggregate." << f->local_name () << ".in () ? "
            << "ACE_OS::strlen(_tao_aggregate."
            << f->local_name () << ".in ()) : 0)";
      }
      break;
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_aggregate." << f->local_name () << ".out ())";

      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_aggregate." << f->local_name () << ".in ())";

      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // Nothing to be done.
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_array - "
                         "bad substate\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_structure (be_structure *node)
{
  // If the struct is defined in this scope, generate its Serializer stream
  // operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_structure_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_cs::"
                             "visit_struct - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // How generate the marshaling code for the struct as a field.

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_structure - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      *os << "_tao_is_bounded_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      *os << "_dcps_max_marshaled_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << "_dcps_find_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_structure - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_field_serializer_op_cs::visit_union (be_union *node)
{
  // If the union is defined in this scope, generate its Serializer stream
  // operators here.
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      be_visitor_union_serializer_op_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_cs::"
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_union - "
                         "cannot retrieve field node\n"),
                        -1);
    }

  // Check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      *os << "_tao_is_bounded_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
      *os << "_dcps_max_marshaled_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
    case TAO_CodeGen::TAO_FIND_SIZE:
      *os << "_dcps_find_size (_tao_aggregate." << f->local_name ()
          << ")";
      break;
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
                         "(%N:%l) be_visitor_field_serializer_op_cs::"
                         "visit_union - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}

// ****************************************************************

be_visitor_serializer_op_field_decl::be_visitor_serializer_op_field_decl (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

// This is a field, but the action depends on the type of the field,
// use this visitor to detect the type of the field.
// Notice that this is why the parent visitor (who create us) cannot
// do the job, because it may have another purpose for some or all of
// the visit_* methods; in other words, while running a visitor to
// generate TAO::DCPS::Serializer operators for structures we cannot use that one to
// generate the code of each field, because visit_struct already has a
// meaning in that visitor.
int
be_visitor_serializer_op_field_decl::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_serializer_op_field_decl::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // @@ Shouldn't this be saved in the visitor and not the context?!
  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_serializer_op_field_decl::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_serializer_op_field_decl::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the field node.
  be_field *f =
    be_field::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_serializer_op_field_decl::"
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
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
    case TAO_CodeGen::TAO_FIND_SIZE:
    case TAO_CodeGen::TAO_CDR_INPUT:
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << fname << "_forany "
          << "_tao_aggregate_" << f->local_name () << be_idt << be_idt_nl
          << "(const_cast<" << be_idt << be_idt_nl
          << fname << "_slice*> (" << be_nl
          << "_tao_aggregate." << f->local_name () << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl;

      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_serializer_op_field_decl::"
                         "visit_array - "
                         "bad sub state\n"),
                        -1);
    }

  return 0;
}


int
be_visitor_serializer_op_field_decl::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // Save the typedef node for use in code
                            // generation as we visit the base type.

  // The node to be visited in the base primitve type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (bt == 0 || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_serializer_op_field_decl::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}
