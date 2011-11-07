
//=============================================================================
/**
 *  @file    argument_marshal.cpp
 *
 *  $Id$
 *
 *  Visitor to pass arguments to the CDR operators. This one helps in
 *  generating the && and the , at the right place. This one is for the
 *  skeleton side.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************
// operation visitor to handle the passing of arguments to the CDR operators
// ************************************************************

be_visitor_operation_argument_marshal::be_visitor_operation_argument_marshal (
    be_visitor_context
    *ctx
  )
  : be_visitor_operation_argument (ctx),
    last_arg_printed_ (be_visitor_operation_argument_marshal::TAO_ARG_NONE)
{
}

be_visitor_operation_argument_marshal::~be_visitor_operation_argument_marshal (
    void
  )
{
}

int
be_visitor_operation_argument_marshal::pre_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  be_argument *arg = be_argument::narrow_from_decl (bd);

  if (!arg)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_visitor_operation_argument_marshal"
                         "::pre_process - "
                         "Bad argument node\n"),
                        -1);
    }

  switch (arg->direction ())
    {
    case AST_Argument::dir_IN:
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
        {
          if (this->last_arg_printed_ !=
              be_visitor_operation_argument_marshal::TAO_ARG_NONE)
            *os << " &&";
        }
      else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
        {
          // nothing
        }
      break;
    case AST_Argument::dir_INOUT:
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
        {
          if (this->last_arg_printed_ !=
              be_visitor_operation_argument_marshal::TAO_ARG_NONE)
            *os << " &&";
        }
      else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
        {
          if (this->last_arg_printed_ !=
              be_visitor_operation_argument_marshal::TAO_ARG_NONE)
            *os << " &&";
        }
      break;
    case AST_Argument::dir_OUT:
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
        {
          // nothing
        }
      else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
        {
          if (this->last_arg_printed_ !=
              be_visitor_operation_argument_marshal::TAO_ARG_NONE)
            *os << " &&";
        }
      break;
    }

  return 0;
}

int
be_visitor_operation_argument_marshal::post_process (be_decl *bd)
{
  be_argument *arg = be_argument::narrow_from_decl (bd);

  if (!arg)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_visitor_operation_argument_marshal"
                         "::post_process - "
                         "Bad argument node\n"),
                        -1);
    }

  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          // only these arguments get printed
          this->last_arg_printed_ =
            be_visitor_operation_argument_marshal::TAO_ARG_IN;
          break;
        case AST_Argument::dir_INOUT:
          // only these arguments get printed
          this->last_arg_printed_ =
            be_visitor_operation_argument_marshal::TAO_ARG_INOUT;
          break;
        case AST_Argument::dir_OUT:
          // these arguments don't get printed for the << operator on the stub
          break;
        }
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          // these arguments don't get printed for the >> on the stub
          break;
        case AST_Argument::dir_INOUT:
          // only these arguments get printed
          this->last_arg_printed_ =
            be_visitor_operation_argument_marshal::TAO_ARG_INOUT;
          break;
        case AST_Argument::dir_OUT:
          // only these arguments get printed
          this->last_arg_printed_ =
            be_visitor_operation_argument_marshal::TAO_ARG_OUT;
          break;
        }
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_visitor_operation_argument_marshal"
                         "::post_process - "
                         "Bad sub state\n"),
                        -1);
    }

  return 0;
}

// ****************************************************************

be_visitor_args_decl::be_visitor_args_decl (be_visitor_context *ctx)
  :  be_visitor_scope (ctx)
{
}

int
be_visitor_args_decl::visit_operation (be_operation *node)
{
  return this->visit_scope (node);
}

int
be_visitor_args_decl::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type of the argument
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  return bt->accept (this);
}

// visit array
int
be_visitor_args_decl::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // retrieve the field node
  be_argument *f =
    be_argument::narrow_from_decl (this->ctx_->node ());

  if (f == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_args_decl::"
                         "visit_array - "
                         "cannot retrieve argument node\n"),
                        -1);
    }

  // for anonymous arrays, the type name has a _ prepended. We compute
  // the full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // For anonymous arrays ...
      // We have to generate a name for us that has an underscope
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

  if (f->direction () != AST_Argument::dir_IN)
    {
      *os << fname << "_forany "
          << "_tao_argument_" << f->local_name () << " ("
          << be_idt << be_idt_nl
          << f->local_name ()
          << be_uidt_nl << ");" << be_uidt_nl;
    }

  return 0;
}

// visit typedef type
int
be_visitor_args_decl::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // The node to be visited in the base primitve
  // type that gets typedefed.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_args_decl::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}
