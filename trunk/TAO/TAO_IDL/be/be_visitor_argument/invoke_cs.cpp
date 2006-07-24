//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    invoke_cs.cpp
//
// = DESCRIPTION
//    Visitor generating the code that passes arguments to the CDR operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_argument,
           invoke_cs,
           "$Id$")


// ****************************************************************************
// visitor for arguments passing to the CDR operators.
// ****************************************************************************

be_visitor_args_invoke_cs::
be_visitor_args_invoke_cs (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_invoke_cs::
~be_visitor_args_invoke_cs (void)
{
}

int be_visitor_args_invoke_cs::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << "(_tao_out << ";
          break;
       case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << "(_tao_in >> ";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << ")";
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << ")";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << node->name () << "_forany ("
              << "(" << node->name () << "_slice *)"
              << arg->local_name () << ")";
          break;
        case AST_Argument::dir_INOUT:
          *os << node->name () << "_forany ("
              << arg->local_name () << ")";
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << "_tao_argument_" << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_enum - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_interface (be_interface *)
{
  return this->emit_common ();
}

int be_visitor_args_invoke_cs::visit_interface_fwd (be_interface_fwd *)
{
  return this->emit_common ();
}

int be_visitor_args_invoke_cs::visit_valuebox (be_valuebox *)
{
  return this->emit_common ();
}

int be_visitor_args_invoke_cs::visit_valuetype (be_valuetype *)
{
  return this->emit_common ();
}

int
be_visitor_args_invoke_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  return this->emit_common ();
}

int be_visitor_args_invoke_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          switch (node->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
            case AST_PredefinedType::PT_object:
            case AST_PredefinedType::PT_any:
            case AST_PredefinedType::PT_long:
            case AST_PredefinedType::PT_ulong:
            case AST_PredefinedType::PT_longlong:
            case AST_PredefinedType::PT_ulonglong:
            case AST_PredefinedType::PT_short:
            case AST_PredefinedType::PT_ushort:
            case AST_PredefinedType::PT_float:
            case AST_PredefinedType::PT_double:
            case AST_PredefinedType::PT_longdouble:
              *os << arg->local_name ();
              break;
            case AST_PredefinedType::PT_char:
              *os << "::ACE_OutputCDR::from_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "::ACE_OutputCDR::from_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "::ACE_OutputCDR::from_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "::ACE_OutputCDR::from_octet (" << arg->local_name () << ")";
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_args_invoke_cs::"
                                 "visit_predefined_type - "
                                 "Bad predefined type\n"),
                                -1);
            }
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
          switch (node->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
            case AST_PredefinedType::PT_object:
            case AST_PredefinedType::PT_any:
            case AST_PredefinedType::PT_long:
            case AST_PredefinedType::PT_ulong:
            case AST_PredefinedType::PT_longlong:
            case AST_PredefinedType::PT_ulonglong:
            case AST_PredefinedType::PT_short:
            case AST_PredefinedType::PT_ushort:
            case AST_PredefinedType::PT_float:
            case AST_PredefinedType::PT_double:
            case AST_PredefinedType::PT_longdouble:
              *os << arg->local_name ();
              break;
            case AST_PredefinedType::PT_char:
              *os << "::ACE_InputCDR::to_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "::ACE_InputCDR::to_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "::ACE_InputCDR::to_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "::ACE_InputCDR::to_octet (" << arg->local_name () << ")";
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_operation_rettype_compiled_marshal_cs::"
                                 "visit_array - "
                                 "Bad predefined type\n"),
                                -1);
            }
           break;
        case AST_Argument::dir_OUT:
          switch (node->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
            case AST_PredefinedType::PT_object:
              *os << arg->local_name () << ".ptr ()";
              break;
            case AST_PredefinedType::PT_any:
              *os << "*" << arg->local_name () << ".ptr ()";
              break;
            case AST_PredefinedType::PT_long:
            case AST_PredefinedType::PT_ulong:
            case AST_PredefinedType::PT_longlong:
            case AST_PredefinedType::PT_ulonglong:
            case AST_PredefinedType::PT_short:
            case AST_PredefinedType::PT_ushort:
            case AST_PredefinedType::PT_float:
            case AST_PredefinedType::PT_double:
            case AST_PredefinedType::PT_longdouble:
              *os << arg->local_name ();
              break;
            case AST_PredefinedType::PT_char:
              *os << "::ACE_InputCDR::to_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "::ACE_InputCDR::to_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "::ACE_InputCDR::to_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "::ACE_InputCDR::to_octet (" << arg->local_name () << ")";
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_args_invoke_cs::"
                                 "visit_array - "
                                 "Bad predefined type\n"),
                                -1);
            }
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          *os << "*" << arg->local_name () << ".ptr ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      // We need to make a distinction between bounded and unbounded strings.
      if (node->max_size ()->ev ()->u.ulval == 0)
        {
          // Unbounded.
          switch (this->direction ())
            {
            case AST_Argument::dir_IN:
              break;
            case AST_Argument::dir_INOUT:
              *os << arg->local_name ();
              break;
            case AST_Argument::dir_OUT:
              *os << arg->local_name () << ".ptr ()";
              break;
            }
        }
      else
        {
          // Bounded.
          switch (this->direction ())
            {
            case AST_Argument::dir_IN:
              break;
            case AST_Argument::dir_INOUT:
              if (node->width () == (long) sizeof (char))
                {
                  *os << "::ACE_InputCDR::to_string (";
                }
              else
                {
                  *os << "::ACE_InputCDR::to_wstring (";
                }

              *os << arg->local_name () << ", "
                  << node->max_size ()->ev ()->u.ulval << ")";
              break;
            case AST_Argument::dir_OUT:
              if (node->width () == (long) sizeof (char))
                {
                  *os << "::ACE_InputCDR::to_string (";
                }
              else
                {
                  *os << "::ACE_InputCDR::to_wstring (";
                }

              *os << arg->local_name () << ".ptr (), "
                  << node->max_size ()->ev ()->u.ulval << ")";
              break;
            }
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_structure (be_structure *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_invoke_cs::visit_union (be_union *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_invoke_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_args_invoke_cs::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

int
be_visitor_args_invoke_cs::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_args_invoke_cs::visit_eventtype (
    be_eventtype *node
  )
{
  return this->visit_valuetype (node);
}

int
be_visitor_args_invoke_cs::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_args_invoke_cs::visit_home (
    be_home *node
  )
{
  return this->visit_interface (node);
}


int be_visitor_args_invoke_cs::emit_common (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".ptr ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "emit_common - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::emit_common2 (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          if (node->size_type () == AST_Type::VARIABLE)
            *os << "*" << arg->local_name () << ".ptr ()";
          else
            *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_invoke_cs::"
                         "emit_common2 - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}
