//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    vardecl_ss.cpp
//
// = DESCRIPTION
//    Visitor that generates the variable declaration in the skeleton
//    corresponding to the Argument node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_argument, 
           vardecl_ss, 
           "$Id$")


// ************************************************************************
// Visitor to generate code for argument variable declaration
// ************************************************************************

be_visitor_args_vardecl_ss::be_visitor_args_vardecl_ss (
    be_visitor_context *ctx
  )
  : be_visitor_args (ctx)
{
}

be_visitor_args_vardecl_ss::~be_visitor_args_vardecl_ss (void)
{
}

int be_visitor_args_vardecl_ss::visit_argument (be_argument *node)
{
  this->ctx_->node (node);
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl;

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << " " << arg->local_name () << ";" << be_nl
          << bt->name () << "_forany _tao_forany_"
          << arg->local_name () << " (" << be_idt << be_idt_nl
          << arg->local_name () << be_uidt_nl
          << ");" << be_uidt;

      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_var " << arg->local_name ()
              << ";";
        }
      else
        {
          *os << bt->name () << " " << arg->local_name ()
              << ";";
        }

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << bt->name () << " " << arg->local_name () << ";";

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << "_var " << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      *os << bt->name () << "_var "
          << arg->local_name () << ";";

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << "_var " << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      *os << bt->name () << "_var "
          << arg->local_name () << ";";

      break;
    }

  return 0;
}


int be_visitor_args_vardecl_ss::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << "_var " << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      *os << bt->name () << "_var "
          << arg->local_name () << ";";

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << "_var " << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      *os << bt->name () << "_var "
          << arg->local_name () << ";";

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << bt->name () << " " << arg->local_name () << ";";

          break;
        case AST_Argument::dir_OUT:
          *os << bt->name () << "_var "
              << arg->local_name () << ";";

          break;
        }
    }
  else if (pt == AST_PredefinedType::PT_pseudo
           || pt == AST_PredefinedType::PT_object)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          *os << bt->name () << "_var " << arg->local_name ()
              << ";";
          break;
        case AST_Argument::dir_OUT:
          *os << bt->name () << "_var "
              << arg->local_name () << ";";

          break;
        }
    }
  else
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << bt->name () << " " << arg->local_name ();

          // @@@ (JP) This is a hack for VC7, which gets an internal
          // compiler error if a boolean OUT is not initialized.
          // (02-08-13)
          if (this->direction () == AST_Argument::dir_OUT
              && pt == AST_PredefinedType::PT_boolean)
            {
              *os << " = 0";
            }

          *os << ";";

          break;
        }
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << " " << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      *os << bt->name () << "_var "
          << arg->local_name () << ";" << be_nl;

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      if (node->width () == (long) sizeof (char))
        {
          *os << "CORBA::String_var " << arg->local_name () << ";";
        }
      else
        {
          *os << "CORBA::WString_var " << arg->local_name () << ";";
        }

      break;
    case AST_Argument::dir_OUT:
      if (node->width () == (long )sizeof (char))
        {
          *os << "CORBA::String_var " << arg->local_name () << ";";
        }
      else
        {
          *os << "CORBA::WString_var " << arg->local_name () << ";";
        }

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << " " << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_var "
              << arg->local_name () << ";";
        }
      else
        {
          *os << bt->name () << " " << arg->local_name () << ";";
        }

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << bt->name () << " " << arg->local_name () << ";";

      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_var "
              << arg->local_name () << ";";

        }
      else
        {
          *os << bt->name () << " " << arg->local_name () << ";";
        }

      break;
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_args_vardecl_ss::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

int
be_visitor_args_vardecl_ss::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_args_vardecl_ss::visit_eventtype (
    be_eventtype *node
  )
{
  return this->visit_valuetype (node);
}

int
be_visitor_args_vardecl_ss::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}


