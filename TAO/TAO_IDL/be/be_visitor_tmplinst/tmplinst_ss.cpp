//=============================================================================
/**
*  @file   be_visitor_tmplinst_ss.cpp
*
*  $Id$
*
*  This visitor generates explicit template instantiations, guarded so they
*  are seen only by compilers whose platforms require explicit template
*  instantiation.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

be_visitor_tmplinst_ss::be_visitor_tmplinst_ss (be_visitor_context *ctx)
  : be_visitor_tmplinst (ctx)
{
}

be_visitor_tmplinst_ss::~be_visitor_tmplinst_ss (void)
{
}

int
be_visitor_tmplinst_ss::visit_interface (be_interface *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes. This must come before
  // the check for an imported or undefined node because such a node
  // could still be used in an operation and thus cause the use of
  // an arg helper template class.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::SArg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Object_SArg_Traits_T<" << this->linebreak_
          << be_idt << be_idt_nl
          << node->name () << "_ptr," << this->linebreak_ << be_nl
          << node->name () << "_var," << this->linebreak_ << be_nl
          << node->name () << "_out," << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt << be_uidt;

      os->gen_endif ();
    }

  // If one of these is true we can skip the rest.
  if (node->imported () || !node->is_defined ())
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_interface - visit scope failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_ss::visit_valuetype (be_valuetype *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Arg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Object_SArg_Traits_T<" << this->linebreak_
          << be_idt << be_idt_nl
          << node->name () << " *," << this->linebreak_ << be_nl
          << node->name () << "_var," << this->linebreak_ << be_nl
          << node->name () << "_out," << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt << be_uidt;

      os->gen_endif ();
    }

  if (node->imported () || !node->is_defined ())
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_valuetype - visit scope failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_operation (be_operation *node)
{
  if (this->this_mode_generated (node) 
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  const char * S = "S";

  be_visitor_arg_tmplinst visitor (this->ctx_,
                                   this->mode_,
                                   this->prefix_,
                                   this->suffix_,
                                   this->linebreak_,
                                   S);
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  visitor.direction ();

  if (bt->accept (&visitor) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_operation - "
                         "codegen for return type failed\n"), 
                        -1);
    }

  be_argument *arg = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      arg = be_argument::narrow_from_decl (si.item ());
      visitor.direction (arg->direction ());
      bt = be_type::narrow_from_decl (arg->field_type ());

      if (bt->accept (&visitor) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_tmplinst_ss::"
                             "visit_operation - "
                             "codegen for argument failed\n"), 
                            -1);
        }
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_sequence (be_sequence *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *alias = this->ctx_->alias ();

  // For arg/return type helper template classes.
  // If the node is anonymous, it can't have been seen in an operation,
  // so no check is necessary for alias == 0.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::SArg_Traits<" << alias->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Var_Size_SArg_Traits_T<" << this->linebreak_
          << be_idt << be_idt_nl
          << alias->name () << "," << this->linebreak_ << be_nl
          << alias->name () << "_var," << this->linebreak_ << be_nl
          << alias->name () << "_out" << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

        os->gen_endif ();
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_string (be_string *node)
{
  if (this->this_mode_generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  unsigned long bound = node->max_size ()->ev ()->u.ulval;
  be_typedef *alias = this->ctx_->alias ();

  // Unbounded (w)string args are handled as a predefined type.
  // Bounded (w)strings must come in as a typedef - they can't
  // be used directly as arguments or return types.
  if (bound == 0 || alias == 0)
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  // For arg/return type helper template classes.

  idl_bool wide = (node->width () != 1);

  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::SArg_Traits<TAO::" << alias->local_name ()
      << "_" << bound << ">" << this->suffix_;

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::BD_" << (wide ? "W" : "")
      << "String_SArg_Traits<" << bound << ">" << this->suffix_;

  os->gen_endif ();

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_array (be_array *node)
{
  if (this->this_mode_generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes.
  idl_bool variable = (node->size_type () == AST_Type::VARIABLE);

  os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

  *os << be_nl << be_nl << this->prefix_ << " TAO::SArg_Traits<"
      << node->name () << ">" << this->suffix_;

  *os << be_nl << be_nl
      << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
      << "TAO::" << (variable ? "Var" : "Fixed") << "_Array_SArg_Traits_T<"
      << this->linebreak_ << be_idt << be_idt_nl
      << node->name () << "," << this->linebreak_ << be_nl
      << node->name () << "_slice," << this->linebreak_ << be_nl
      << node->name () << "_var," << this->linebreak_ << be_nl;

  if (variable)
    {
      *os << node->name () << "_out," << this->linebreak_ << be_nl;
    }

  *os << node->name () << "_forany" << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  os->gen_endif ();

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_attribute (be_attribute *node)
{
  if (this->this_mode_generated (node)
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  const char * S = "S";

  be_visitor_arg_tmplinst visitor (this->ctx_,
                                   this->mode_,
                                   this->prefix_,
                                   this->suffix_,
                                   this->linebreak_,
                                   S);
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  visitor.direction ();

  if (bt->accept (&visitor) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_attribute - "
                         "codegen for return type failed\n"),
                        -1);
    }

  if (!node->readonly ())
    {
      visitor.direction (AST_Argument::dir_IN);

      if (bt->accept (&visitor) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_tmplinst_ss::"
                             "visit_attribute - "
                             "codegen for IN parameter failed\n"),
                            -1);
        }
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_enum (be_enum *node)
{
  if (this->this_mode_generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes.
  os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::SArg_Traits<" << node->name ()
      << ">" << this->suffix_;

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::Basic_SArg_Traits_T<" << node->name ()
      << ">" << this->suffix_;

  os->gen_endif ();

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_structure (be_structure *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes.
  if (node->seen_in_operation ())
    {
      idl_bool variable = (node->size_type () == AST_Type::VARIABLE);

      os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::SArg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::" << (variable ? "Var" : "Fixed") << "_Size_SArg_Traits_T<"
          << this->linebreak_ << be_idt << be_idt_nl
          << node->name ();

      if (variable)
        {
          *os << "," << this->linebreak_ << be_nl
              << node->name () << "_var," << this->linebreak_ << be_nl
              << node->name () << "_out";
        }

      *os << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_structure - visit scope failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_union (be_union *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (node->seen_in_operation ())
    {
      idl_bool variable = (node->size_type () == AST_Type::VARIABLE);

      os->gen_ifdef_macro (node->flat_name (), "sarg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::SArg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::" << (variable ? "Var" : "Fixed") << "_Size_SArg_Traits_T<"
          << this->linebreak_ << be_idt << be_idt_nl
          << node->name ();

      if (variable)
        {
          *os << "," << this->linebreak_ << be_nl
              << node->name () << "_var," << this->linebreak_ << be_nl
              << node->name () << "_out";
        }

      *os << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_structure - visit scope failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

idl_bool
be_visitor_tmplinst_ss::this_mode_generated (be_decl *node) const
{
  switch (this->mode_)
    {
      case TMPL_CLASS:
        return node->srv_tmpl_class_gen ();
      case PRAGMA_INST:
        return node->srv_pragma_inst_gen ();
      default:
        return I_FALSE;
    }
}

void
be_visitor_tmplinst_ss::this_mode_generated (be_decl *node,
                                             idl_bool val)
{
  switch (this->mode_)
    {
      case TMPL_CLASS:
        node->srv_tmpl_class_gen (val);
        break;
      case PRAGMA_INST:
        node->srv_pragma_inst_gen (val);
        break;
      default:
        break;
    }
}

