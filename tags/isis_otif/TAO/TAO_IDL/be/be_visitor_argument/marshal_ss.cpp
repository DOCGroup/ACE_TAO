//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    marshal_ss.cpp
//
// = DESCRIPTION
//    Visitor that generates code that passes the argument variable to the
//    marshal operations.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_argument, 
           marshal_ss, 
           "$Id$")


// ************************************************************************
// Visitor to generate code for passing argument to the marshal/demarshal
// routines
// ************************************************************************

be_visitor_args_marshal_ss::
be_visitor_args_marshal_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_marshal_ss::
~be_visitor_args_marshal_ss (void)
{
}

int be_visitor_args_marshal_ss::visit_argument (be_argument *node)
{
  this->ctx_->node (node);
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  TAO_OutStream *os = this->ctx_->stream ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << be_nl << "(_tao_in >> ";
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << be_nl << "(_tao_out << ";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
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
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
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
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "_tao_forany_" << arg->local_name ();
          break;
        case AST_Argument::dir_INOUT:
          *os << "_tao_forany_" << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
          *os << "_tao_forany_" << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          *os << "_tao_forany_" << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
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
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
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
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_enum - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name () << ".out ()";
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".in ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name () << ".out ()";
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".in ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_interface_fwd - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name () << ".out ()";
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".in ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_valuetype - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << arg->local_name () << ".out ()";
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".in ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_valuetype_fwd - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_predefined_type (
    be_predefined_type *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          switch (node->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
            case AST_PredefinedType::PT_object:
              *os << arg->local_name () << ".out ()";
              break;
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
                                 "be_visitor_args_compiled_marshal_ss::"
                                 "visit_predefined_type - "
                                 "Bad predefined type\n"),
                                -1);
            }
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
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
              *os << arg->local_name () << ".in ()";
              break;
            case AST_PredefinedType::PT_any:
              *os << arg->local_name ();
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
                                 "be_visitor_operation_rettype_compiled_marshal_ss::"
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
              *os << arg->local_name () << ".in ()";
              break;
            case AST_PredefinedType::PT_any:
              *os << arg->local_name () << ".in ()";
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
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  "be_visitor_operation_rettype_compiled_marshal_ss::"
                  "visit_array - "
                  "Bad predefined type\n"
                ),
                -1
              );
            }
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
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
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
          *os << arg->local_name ();
          break;
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".in ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          // we need to make a distinction between bounded and unbounded strings
          if (node->max_size ()->ev ()->u.ulval == 0)
            {
              *os << arg->local_name () << ".out ()";
            }
          else
            {
              if (node->width () == (long) sizeof (char))
                {
                  *os << "CORBA::Any::to_string (";
                }
              else
                {
                  *os << "CORBA::Any::to_wstring (";
                }

              *os << arg->local_name () << ".out (), "
                  << node->max_size ()->ev ()->u.ulval
                  << ")";
            }
          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
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
            case AST_Argument::dir_OUT:
              *os << arg->local_name () << ".in ()";
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
            case AST_Argument::dir_OUT:
              if (node->width () == (long) sizeof (char))
                {
                  *os << "CORBA::Any::from_string ((char *)";
                }
              else
                {
                  *os << "CORBA::Any::from_wstring ((CORBA::WChar *)";
                }

              *os << arg->local_name () << ".in (), "
                  << node->max_size ()->ev ()->u.ulval << ")";
              break;
            }
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_string - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
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
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
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
            *os << arg->local_name () << ".in ()";
          else
            *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_structure - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
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
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
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
            *os << arg->local_name () << ".in ()";
          else
            *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_union - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_args_marshal_ss::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

int
be_visitor_args_marshal_ss::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_args_marshal_ss::visit_eventtype (
    be_eventtype *node
  )
{
  return this->visit_valuetype (node);
}

int
be_visitor_args_marshal_ss::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_args_marshal_ss::visit_home (
    be_home *node
  )
{
  return this->visit_interface (node);
}



