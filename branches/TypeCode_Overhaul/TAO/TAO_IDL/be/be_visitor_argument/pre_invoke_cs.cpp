//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    pre_invoke_cs.cpp
//
// = DESCRIPTION
//    Visitor that generates code (if any) for pre-processing prior to call to
//    do_static_call in the client stub
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_argument, 
           pre_invoke_cs, 
           "$Id$")


// *************************************************************************
// visitor for argument to do any pre invoke processing. Not all types need
// this. Only those that have an _out type need this.  This ne is for compiled
// marshaling and overrides osme methods of the base class (which does the job
// for interpretive marshaling)
// *************************************************************************

be_visitor_args_pre_invoke_cs::be_visitor_args_pre_invoke_cs
(be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_pre_invoke_cs::~be_visitor_args_pre_invoke_cs (void)
{
}

int
be_visitor_args_pre_invoke_cs::void_return_type (void)
{
  // Get the return type.
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_operation *op = be_operation::narrow_from_scope (arg->defined_in ());
  be_type *bt = be_type::narrow_from_decl (op->return_type ());

  // Is the operation return type void?
  if (bt->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (bt)->pt ()
          == AST_PredefinedType::PT_void))
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

int
be_visitor_args_pre_invoke_cs::cannot_return_zero (void)
{
  // Get the return type.
  be_argument *arg = this->ctx_->be_node_as_argument ();
  be_operation *op = be_operation::narrow_from_scope (arg->defined_in ());
  be_type *bt = be_type::narrow_from_decl (op->return_type ());

  // Two types qualify - enum and fixed size struct.
  if (bt->base_node_type () == AST_Decl::NT_enum)
    {
      return 1;
    }
  else if (bt->base_node_type () == AST_Decl::NT_struct
           && bt->size_type () == AST_Type::FIXED)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

int be_visitor_args_pre_invoke_cs::visit_argument (be_argument *node)
{
  this->ctx_->node (node);

  // Retrieve the type of the argument.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_docall_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_docall_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  // If the current type is an alias, use that.
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
    case AST_Argument::dir_OUT:
      if (node->size_type () == AST_Type::VARIABLE)
        {
          os->indent ();

          if (!this->void_return_type ())
            {
              *os << "ACE_ALLOCATOR_RETURN (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << bt->name () << "_alloc ()," << be_nl;

              if (this->cannot_return_zero ())
                {
                  *os <<"_tao_retval";
                }
              else
                {
                  *os << "0";
                }
            }
          else
            {
              *os << "ACE_ALLOCATOR (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << bt->name () << "_alloc ()";
            }

          *os << be_uidt_nl << ");" << be_uidt_nl << "\n";
        }

      break;
    default:
      break;
    }

  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_interface (be_interface *)
{
  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();


  // Pre_invoke processing is valid only for pseudo objects and for Any.
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          break;
        default:
          break;
        }
      }
      break;
    case AST_PredefinedType::PT_any:
      {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          os->indent ();

          if (!this->void_return_type ())
            {
              *os << be_nl << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << "CORBA::Any," << be_nl;

              if (this->cannot_return_zero ())
                {
                  *os << "_tao_retval";
                }
              else
                {
                  *os << "0";
                }
            }
          else
            {
              *os << "ACE_NEW (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << "CORBA::Any";
            }

          *os << be_uidt_nl << ");" << be_uidt_nl;

          break;
        default:
          break;
        }
      }
      break;
    default:
      break;
    }

  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  // If the current type is an alias, use that.
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
    case AST_Argument::dir_OUT:
      os->indent ();

      if (!this->void_return_type ())
        {
          *os << be_nl << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
              << arg->local_name () << ".ptr ()," << be_nl
              << bt->name () << "," << be_nl;

          if (this->cannot_return_zero ())
            {
              *os << "_tao_retval";
            }
          else
            {
              *os << "0";
            }
        }
      else
        {
          *os << "ACE_NEW (" << be_idt << be_idt_nl
              << arg->local_name () << ".ptr ()," << be_nl
              << bt->name ();
        }

      *os << be_uidt_nl << ");" << be_uidt_nl;

      break;
    default:
      break;
    }

  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_string (be_string *)
{
  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  // If the current type is an alias, use that.
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (node->size_type () == be_type::VARIABLE)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          os->indent ();

          if (!this->void_return_type ())
            {
              *os << be_nl << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << bt->name () << "," << be_nl;

              if (this->cannot_return_zero ())
                {
                  *os << "_tao_retval";
                }
              else
                {
                  *os << "0";
                }
            }
          else
            {
              *os << "ACE_NEW (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << bt->name ();
            }

          *os << be_uidt_nl << ");" << be_uidt_nl;

          break;
        default:
          break;
        }
    }

  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  // If the current type is an alias, use that.
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (node->size_type () == be_type::VARIABLE)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_OUT:
          os->indent ();

          if (!this->void_return_type ())
            {
              *os << be_nl << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << bt->name () << "," << be_nl;

              if (this->cannot_return_zero ())
                {
                  *os << "_tao_retval";
                }
              else
                {
                  *os << "0";
                }
            }
          else
            {
              *os << "ACE_NEW (" << be_idt << be_idt_nl
                  << arg->local_name () << ".ptr ()," << be_nl
                  << bt->name ();
            }

          *os << be_uidt_nl << ");" << be_uidt_nl;

          break;
        default:
          break;
        }
    }

  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_pre_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_args_pre_invoke_cs::visit_component (
    be_component *node
  )
{
  return this->visit_interface (node);
}

int
be_visitor_args_pre_invoke_cs::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_args_pre_invoke_cs::visit_eventtype (
    be_eventtype *node
  )
{
  return this->visit_valuetype (node);
}

int
be_visitor_args_pre_invoke_cs::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_args_pre_invoke_cs::visit_home (
    be_home *node
  )
{
  return this->visit_interface (node);
}



