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

  // retrieve the type of the argument
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_cs::"
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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
          *os << "_tao_argument_" << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          if (node->size_type () == AST_Type::VARIABLE)
            {
              *os << "_tao_argument_" << arg->local_name ();
            }
          else
            {
              *os << "_tao_argument_" << arg->local_name ();
            }
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_enum - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_valuetype (be_valuetype *)
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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_valuetype - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_args_invoke_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_valuetype - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get argument node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          switch (node->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
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
              *os << "CORBA::Any::from_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "CORBA::Any::from_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "CORBA::Any::from_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "CORBA::Any::from_octet (" << arg->local_name () << ")";
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_args_compiled_marshal_cs::"
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
              *os << "CORBA::Any::to_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "CORBA::Any::to_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "CORBA::Any::to_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "CORBA::Any::to_octet (" << arg->local_name () << ")";
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
              *os << "CORBA::Any::to_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "CORBA::Any::to_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "CORBA::Any::to_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "CORBA::Any::to_octet (" << arg->local_name () << ")";
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_operation_rettype_compiled_marshal_cs::"
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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
      // we need to make a distinction between bounded and unbounded strings
      if (node->max_size ()->ev ()->u.ulval == 0)
        {
          // unbounded
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
          // bounded
          switch (this->direction ())
            {
            case AST_Argument::dir_IN:
              break;
            case AST_Argument::dir_INOUT:
              if (node->width () == (long) sizeof (char))
                {
                  *os << "CORBA::Any::to_string (";
                }
              else
                {
                  *os << "CORBA::Any::to_wstring (";
                }

              *os << arg->local_name () << ", "
                  << node->max_size ()->ev ()->u.ulval << ")";
              break;
            case AST_Argument::dir_OUT:
              if (node->width () == (long) sizeof (char))
                {
                  *os << "CORBA::Any::to_string (";
                }
              else
                {
                  *os << "CORBA::Any::to_wstring (";
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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_invoke_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

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
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_invoke_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
