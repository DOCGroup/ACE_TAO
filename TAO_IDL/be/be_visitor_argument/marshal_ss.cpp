
//=============================================================================
/**
 *  @file    marshal_ss.cpp
 *
 *  $Id$
 *
 *  Visitor that generates code that passes the argument variable to the
 *  marshal operations.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

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

int be_visitor_args_marshal_ss::visit_argument (
  be_argument *node)
{
  this->ctx_->node (node);
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("Bad argument type\n")),
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
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("cannot accept visitor\n")),
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
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_argument - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  const char *lname = arg->local_name ()->get_string ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << "_tao_forany_" << lname;
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
          *os << "_tao_forany_" << lname;
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_array - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  const char *lname = arg->local_name ()->get_string ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << lname;
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
          *os << lname;
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_enum - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_interface (
  be_interface *)
{
  return this->emit_common ();
}

int be_visitor_args_marshal_ss::visit_interface_fwd (
  be_interface_fwd *)
{
  return this->emit_common ();
}

int be_visitor_args_marshal_ss::visit_valuebox (
  be_valuebox *)
{
  return this->emit_common ();
}

int be_visitor_args_marshal_ss::visit_valuetype (
  be_valuetype *)
{
  return this->emit_common ();
}

int be_visitor_args_marshal_ss::visit_valuetype_fwd (
  be_valuetype_fwd *)
{
  return this->emit_common ();
}

int be_visitor_args_marshal_ss::visit_predefined_type (
  be_predefined_type *node)
{
  return this->gen_pd_arg (node, false);
}

int be_visitor_args_marshal_ss::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  const char *lname = arg->local_name ()->get_string ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << lname;
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
          *os << lname;
          break;
        case AST_Argument::dir_OUT:
          *os << lname << ".in ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  const char *lname = arg->local_name ()->get_string ();
  ACE_CDR::ULong bound = node->max_size ()->ev ()->u.ulval;
  bool wide =
    (node->width () != static_cast<long> (sizeof (char)));

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          if (bound == 0)
            {
              *os << lname << ".out ()";
            }
          else
            {
              *os << "::ACE_InputCDR::to_"
                  << (wide ? "w" : "" ) << "string ("
                  << lname << ".out (), " << bound << ")";
            }

          break;
        case AST_Argument::dir_OUT:
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      // We need to make a distinction between
      // bounded and unbounded strings.
      if (bound == 0)
        {
          switch (this->direction ())
            {
            case AST_Argument::dir_IN:
              break;
            case AST_Argument::dir_INOUT:
            case AST_Argument::dir_OUT:
              *os << lname << ".in ()";
              break;
            }
        }
      else
        {
          switch (this->direction ())
            {
            case AST_Argument::dir_IN:
              break;
            case AST_Argument::dir_INOUT:
            case AST_Argument::dir_OUT:
              *os << "::ACE_OutputCDR::from_"
                  << (wide ? "w" : "" ) << "string (("
                  << (wide ? "CORBA::WChar" : "char") << " *)"
                  << lname << ".in (), " << bound << ")";

              break;
            }
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_string - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  return 0;
}

int be_visitor_args_marshal_ss::visit_structure (be_structure *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_marshal_ss::visit_union (be_union *node)
{
  return this->emit_common2 (node);
}

int be_visitor_args_marshal_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss::")
                         ACE_TEXT ("visit_typedef - ")
                         ACE_TEXT ("accept on primitive ")
                         ACE_TEXT ("type failed\n")),
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

int
be_visitor_args_marshal_ss::emit_common (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  const char *lname = arg->local_name ()->get_string ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << lname << ".out ()";
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
          *os << lname << ".in ()";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss")
                         ACE_TEXT ("::emit_common - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  return 0;
}


int
be_visitor_args_marshal_ss::emit_common2 (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg =
    be_argument::narrow_from_decl (this->ctx_->node ());
  const char *lname = arg->local_name ()->get_string ();
  AST_Type::SIZE_TYPE st = node->size_type ();

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << lname;
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
          *os << lname;
          break;
        case AST_Argument::dir_OUT:
          *os << lname
              << (st == AST_Type::VARIABLE ? ".in ()" : "");

          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_args_marshal_ss")
                         ACE_TEXT ("::emit_common2 - ")
                         ACE_TEXT ("Bad substate\n")),
                        -1);
    }

  return 0;
}
