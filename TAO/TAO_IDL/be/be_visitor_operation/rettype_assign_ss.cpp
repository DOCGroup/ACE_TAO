//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_assign_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for assigning the return value of upcall to the
//    return type variable.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           rettype_assign_ss, 
           "$Id$")

// ****************************************************************************
// visitor for assignment to a return value variable from the upcall
// ****************************************************************************

be_visitor_operation_rettype_assign_ss::
be_visitor_operation_rettype_assign_ss (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_assign_ss::
~be_visitor_operation_rettype_assign_ss (void)
{
}

int
be_visitor_operation_rettype_assign_ss::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";
  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::
visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::
visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_void:
      break;
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_any:
    default:
      *os << "_tao_retval = ";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_structure (be_structure *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_assign_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_union (be_union *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}
