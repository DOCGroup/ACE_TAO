//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_pre_invoke_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for doing any pre-processing of return type prior
//    to making the invocation.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, rettype_pre_invoke_cs, "$Id$")


// *****************************************************************************
//    be_visitor_operation_rettype_pre_invoke_cs
// *****************************************************************************

be_visitor_operation_rettype_pre_invoke_cs::
be_visitor_operation_rettype_pre_invoke_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_pre_invoke_cs::
~be_visitor_operation_rettype_pre_invoke_cs (void)
{
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << "{" << be_idt_nl
      << bt->name () << "_slice *tmp;" << be_nl
      << "ACE_ALLOCATOR_RETURN (tmp, "
      << bt->name () << "_alloc (), _tao_retval._retn ());" << be_nl
      << "_tao_retval = tmp;" << be_uidt_nl
      << "}\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_interface (be_interface *)
{
  // don't do anything. This is the overriding action
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_any:
      os->indent ();
      *os << "{" << be_idt_nl
          << "CORBA::Any *tmp;" << be_nl
          << "ACE_NEW_RETURN (tmp, CORBA::Any, _tao_retval._retn ());" << be_nl
          << "_tao_retval = tmp;" << be_uidt_nl
          << "}\n";
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << "{" << be_idt_nl
      << bt->name () << " *tmp;" << be_nl
      << "ACE_NEW_RETURN (tmp, "
      << bt->name () << ", _tao_retval._retn ());" << be_nl
      << "_tao_retval = tmp;" << be_uidt_nl
      << "}\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  // check if the union is variable
  if (node->size_type () == be_type::VARIABLE)
    {
      os->indent ();
      *os << "{" << be_idt_nl
          << bt->name () << " *tmp;" << be_nl
          << "ACE_NEW_RETURN (tmp, "
          << bt->name () << ", _tao_retval._retn ());" << be_nl
          << "_tao_retval = tmp;" << be_uidt_nl
          << "}\n";
    }
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_pre_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_pre_invoke_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  // check if the union is variable
  if (node->size_type () == be_type::VARIABLE)
    {
      os->indent ();
      *os << "{" << be_idt_nl
          << bt->name () << " *tmp;" << be_nl
          << "ACE_NEW_RETURN (tmp, "
          << bt->name () << ", _tao_retval._retn ());" << be_nl
          << "_tao_retval = tmp;" << be_uidt_nl
          << "}\n";
    }
  return 0;
}

