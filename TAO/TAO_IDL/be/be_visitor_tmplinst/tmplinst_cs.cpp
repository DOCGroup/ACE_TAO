//=============================================================================
/**
*  @file   be_visitor_tmplinst_cs.cpp
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

be_visitor_tmplinst_cs::be_visitor_tmplinst_cs (be_visitor_context *ctx)
  : be_visitor_tmplinst (ctx)
{
}

be_visitor_tmplinst_cs::~be_visitor_tmplinst_cs (void)
{
}

int
be_visitor_tmplinst_cs::visit_interface (be_interface *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Arg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl 
          << "TAO::Object_Arg_Traits_T<" << this->linebreak_ 
          << be_idt << be_idt_nl
          << node->name () << "_ptr," << this->linebreak_ << be_nl
          << node->name () << "_var," << this->linebreak_ << be_nl
          << node->name () << "_out" << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_interface - visit scope failed\n"),
                        -1);
    }

  if (node->imported ())
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  // For traits template class.
  // @@@ (JP) This condition may change or go away once we 
  // regenerate the ORB hand-crafted code.
  if (!node->is_defined ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Objref_Traits<" << node->name ()
          << ">" << this->suffix_;
    }

  // For _var and _out template classes.
  if (node->is_defined ())
    {
      *os << be_nl << be_nl
          << this->prefix_<< this->linebreak_ << be_idt << be_idt_nl
          << "TAO_Objref_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
          << node->name () << "," << this->linebreak_ << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">" << this->linebreak_ 
          << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt_nl << be_uidt_nl
          << this->prefix_<< this->linebreak_ << be_idt << be_idt_nl
          << "TAO_Objref_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
          << node->name () << "," << this->linebreak_ << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">" << this->linebreak_ 
          << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

      if (be_global->gen_smart_proxies ())
        {
          // Its necessary to take care of the nested case.
          // The smart proxy classes are in the same scope as the proxy.
          be_decl* scope = 
            be_scope::narrow_from_scope (node->defined_in ())->decl ();

          *os << be_nl << be_nl
              << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
              << "TAO_Singleton<" << this->linebreak_ << be_idt << be_idt_nl
              << scope->full_name ();

          // Only if there exists any nesting "::" is needed!
          if (node->is_nested ())
            {
              *os << "::";
            }

          *os <<"TAO_" << node->flat_name ()
              << "_Proxy_Factory_Adapter," << this->linebreak_ << be_nl
              << "TAO_SYNCH_RECURSIVE_MUTEX" << this->linebreak_ << be_uidt_nl
              << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
        }
    }

  // For Any impl template class.
  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Any_Impl_T<" << this->linebreak_ << be_idt << be_idt_nl
          << node->name () << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_valuetype (be_valuetype *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Arg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl 
          << "TAO::Object_Arg_Traits_T<" << this->linebreak_ 
          << be_idt << be_idt_nl
          << node->name () << " *," << this->linebreak_ << be_nl
          << node->name () << "_var," << this->linebreak_ << be_nl
          << node->name () << "_out" << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_valuetype - visit scope failed\n"),
                        -1);
    }

  if (node->imported ())
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  // For traits template class.
  // @@@ (JP) This condition may change or go away once we 
  // regenerate the ORB hand-crafted code.
  if (!node->is_defined ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Value_Traits<" << node->name ()
          << ">" << this->suffix_;
    }

  // For _var and _out template classes.
  if (node->is_defined ())
    {
      *os << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO_Value_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
          << node->name () << "," << this->linebreak_ << be_nl
          << "TAO::Value_Traits<" << node->name () << ">" << this->linebreak_ 
          << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt_nl << be_uidt_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO_Value_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
          << node->name () << "," << this->linebreak_ << be_nl
          << "TAO::Value_Traits<" << node->name  () << ">" << this->linebreak_
          << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  // For Any impl template class.
  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Any_Impl_T<" << this->linebreak_ << be_idt << be_idt_nl
          << node->name () << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_cs::visit_operation (be_operation *node)
{
  if (node->imported ())
    {
      return 0;
    }

  const char * S = "";

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
                         "(%N:%l) be_visitor_tmplinst_cs::"
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
                             "(%N:%l) be_visitor_tmplinst_cs::"
                             "visit_operation - "
                             "codegen for argument failed\n"), 
                            -1);
        }
    }

  return 0;
}

int
be_visitor_tmplinst_cs::visit_sequence (be_sequence *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *alias = this->ctx_->alias ();

  // For arg/return type helper template classes.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Arg_Traits<" << alias->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Var_Size_Arg_Traits<" << this->linebreak_
          << be_idt << be_idt_nl
          << alias->name () << "," << this->linebreak_ << be_nl
          << alias->name () << "_var," << this->linebreak_ << be_nl
          << alias->name () << "_out" << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

        os->gen_endif ();
    }

  if (node->imported ())
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  // For _var and _out template classes.

  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  AST_Decl::NodeType nt = bt->node_type ();

  if (!bt->seq_elem_tmplinst ())
    {
      // This is a no-op unless our element is a managed type.
      this->gen_managed_type_tmplinst (node, bt);
      bt->seq_elem_tmplinst (I_TRUE);
    }

  if (this->ctx_->alias () != 0)
    {
      this->gen_varout_tmplinst (node,
                                 bt);

      if (nt == AST_Decl::NT_typedef)
        {
          be_typedef *td = be_typedef::narrow_from_decl (bt);
          nt = td->base_node_type ();
        }
    }
      
  // basic IDL types are in TAO. Sequences of (w)strings in TAO are
  // specializations and so are not template classes.
  if ((nt != AST_Decl::NT_pre_defined
       && nt != AST_Decl::NT_string
       && nt != AST_Decl::NT_wstring)
      || ! node->unbounded ())
    {
      if (this->gen_base_class_tmplinst (node) == -1)
        {
          return -1;
        }
    }

  // For Any impl template class.
  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Any_Dual_Impl_T<" << this->linebreak_ 
          << be_idt << be_idt_nl
          << node->name () << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_string (be_string *node)
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

  os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::Arg_Traits<" << alias->local_name ()
      << "_" << bound << ">" << this->suffix_;

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::BD_" << (wide ? "W" : "")
      << "String_Arg_Traits<" << bound << ">" << this->suffix_;

  os->gen_endif ();

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_array (be_array *node)
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

      os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

      *os << be_nl << be_nl << this->prefix_ << " TAO::Arg_Traits<" 
          << node->name () << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::" << (variable ? "Var" : "Fixed") << "_Array_Arg_Traits<"
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
    }

  if (node->imported ())
    {
      return 0;
    }

  // To hold the full name, whether anonymous or not.
  char fname [NAMEBUFSIZE];
  ACE_OS::memset (fname, 
                  '\0', 
                  NAMEBUFSIZE);

  if (this->ctx_->alias ())
    {
      // Typedefed node.
      ACE_OS::sprintf (fname, "%s", 
                       node->full_name ());
    }
  else
    {
      // For anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name.
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

  // For array traits template class.

  *os << be_nl << be_nl
      << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
      << "TAO::Array_Traits<" << this->linebreak_ << be_idt << be_idt_nl
      << fname << "," << this->linebreak_ << be_nl
      << fname << "_slice" << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  // For _var, _out and _forany template classes.

  idl_bool fixed = (node->size_type () == AST_Type::FIXED);

  if (this->ctx_->alias () != 0)
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO_" << (fixed ? "Fixed" : "Var") << "Array_Var_T<" 
          << this->linebreak_ << be_idt << be_idt_nl
          << fname << "," << this->linebreak_ << be_nl
          << fname << "_slice," << this->linebreak_ << be_nl
          << "TAO::Array_Traits<" << this->linebreak_ << be_idt << be_idt_nl
          << fname << "," << this->linebreak_ << be_nl
          << fname << "_slice" << this->linebreak_ << be_uidt_nl
          << ">" << this->linebreak_ << be_uidt << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO_Array_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
          << fname << "," << this->linebreak_ << be_nl
          << fname << "_var," << this->linebreak_ << be_nl
          << fname << "_slice," << this->linebreak_ << be_nl
          << "TAO::Array_Traits<" << this->linebreak_ << be_idt << be_idt_nl
          << fname << "," << this->linebreak_ << be_nl
          << fname << "_slice" << this->linebreak_ << be_uidt_nl
          << ">" << this->linebreak_ << be_uidt << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO_Array_Var_Base_T<" << this->linebreak_ << be_idt 
          << be_idt_nl
          << fname << "," << this->linebreak_ << be_nl
          << fname << "_slice," << this->linebreak_ << be_nl
          << "TAO::Array_Traits<" << this->linebreak_ << be_idt << be_idt_nl
          << fname << "," << this->linebreak_ << be_nl
          << fname << "_slice" << this->linebreak_ << be_uidt_nl
          << ">" << this->linebreak_ << be_uidt << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  *os << be_nl << be_nl
      << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
      << "TAO_Array_Forany_T<" << this->linebreak_ << be_idt << be_idt_nl
      << fname << "," << this->linebreak_ << be_nl
      << fname << "_slice," << this->linebreak_ << be_nl
      << "TAO::Array_Traits<" << this->linebreak_ << be_idt << be_idt_nl
      << fname << "," << this->linebreak_ << be_nl
      << fname << "_slice" << this->linebreak_ << be_uidt_nl
      << ">" << this->linebreak_ << be_uidt << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  // For Any impl template class.
  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Any_Array_Impl_T<" << this->linebreak_ 
          << be_idt << be_idt_nl
          << node->name () << "_slice," << this->linebreak_ << be_nl
          << node->name () << "_forany" << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_enum (be_enum *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // For arg/return type helper template classes.

  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Arg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Basic_Arg_Traits_T<" << node->name ()
          << ">" << this->suffix_;

      os->gen_endif ();
    }

  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Any_Basic_Impl_T<" << this->linebreak_ 
          << be_idt << be_idt_nl
          << node->name () << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_structure (be_structure *node)
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

      os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Arg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::" << (variable ? "Var" : "Fixed") << "_Size_Arg_Traits<"
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
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_structure - visit scope failed\n"),
                        -1);
    }

  if (node->imported ())
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  // For _var and/or _out tmp]ate clases.
  this->gen_common_tmplinst (node, os);

  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Any_Dual_Impl_T<" << this->linebreak_ 
          << be_idt << be_idt_nl
          << node->name () << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_union (be_union *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  if (node->seen_in_operation ())
    {
      idl_bool variable = (node->size_type () == AST_Type::VARIABLE);

      os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Arg_Traits<" << node->name ()
          << ">" << this->suffix_;

      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::" << (variable ? "Var" : "Fixed") << "_Size_Arg_Traits<"
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
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_structure - visit scope failed\n"),
                        -1);
    }

  if (node->imported ())
    {
      this->this_mode_generated (node, I_TRUE);
      return 0;
    }

  // For _var and/or _out tmp]ate clases.
  this->gen_common_tmplinst (node, os);

  if (be_global->any_support ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO::Any_Dual_Impl_T<" << this->linebreak_ 
          << be_idt << be_idt_nl
          << node->name () << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

idl_bool
be_visitor_tmplinst_cs::this_mode_generated (be_decl *node) const
{
  switch (this->mode_)
    {
      case TMPL_CLASS:
        return node->cli_tmpl_class_gen ();
      case PRAGMA_INST:
        return node->cli_pragma_inst_gen ();
      default:
        return I_FALSE;
    }
}

void
be_visitor_tmplinst_cs::this_mode_generated (be_decl *node,
                                             idl_bool val)
{
  switch (this->mode_)
    {
      case TMPL_CLASS:
        node->cli_tmpl_class_gen (val);
        break;
      case PRAGMA_INST:
        node->cli_pragma_inst_gen (val);
        break;
      default:
        break;
    }
}

void
be_visitor_tmplinst_cs::gen_managed_type_tmplinst (be_sequence *node,
                                                   be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->managed_type ())
    {
      case be_sequence::MNG_OBJREF:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Object_Manager<" << this->linebreak_ << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_va," << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_ABSTRACT:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Abstract_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_va," << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_VALUE:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Valuetypet_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_va," << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_PSEUDO:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Pseudo_Object_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

        break;
      default:
        //  String and Wstring managed types are not template classes.
        break;
    }
}

int
be_visitor_tmplinst_cs::gen_varout_tmplinst (be_sequence *node,
                                             be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->managed_type ())
    {
      case be_sequence::MNG_OBJREF:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Object_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ 
            << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Object_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_ 
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << node->name () << "_var," << this->linebreak_ << be_nl
            << "TAO_Object_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_ 
            << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_ABSTRACT:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Abstract_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Abstract_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << node->name () << "_var," << this->linebreak_ << be_nl
            << "TAO_Abstract_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_PSEUDO:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Pseudo_Object_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Pseudo_Object_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << node->name () << "_var," << this->linebreak_ << be_nl
            << "TAO_Pseudo_Object_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var" << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_VALUE:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Valuetype_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var," << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << "TAO_Valuetype_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var," << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << node->name () << "_var," << this->linebreak_ << be_nl
            << "TAO_Valuetype_Manager<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << bt->name () << "," << this->linebreak_ << be_nl
            << bt->name () << "_var," << this->linebreak_ << be_uidt_nl
            << ">" << this->linebreak_ << be_uidt << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_STRING:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            <<"TAO_SeqElem_String_Manager" << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            <<"TAO_SeqElem_String_Manager" << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_ 
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << node->name () << "_var," << this->linebreak_ << be_nl
            <<"TAO_SeqElem_String_Manager" << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        break;
      case be_sequence::MNG_WSTRING:
        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Var_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            <<"TAO_SeqElem_WString_Manager" << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_ 
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_Seq_Var_Base_T<" << this->linebreak_ 
            << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            <<"TAO_SeqElem_WString_Manager" << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_ 
            << be_uidt << be_uidt << be_uidt;

        *os << be_nl << be_nl
            << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
            << "TAO_MngSeq_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
            << node->name () << "," << this->linebreak_ << be_nl
            << node->name () << "_var," << this->linebreak_ << be_nl
            <<"TAO_SeqElem_WString_Manager" << this->linebreak_ << be_uidt_nl
            << ">" << this->suffix_
            << be_uidt << be_uidt << be_uidt;

        break;
      default: // not a managed type
        {
          AST_Type::SIZE_TYPE st = bt->size_type ();

          *os << be_nl << be_nl
              << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
              << (st == AST_Type::FIXED ? "TAO_FixedSeq_Var_T<"
                                        : "TAO_VarSeq_Var_T<")
              << this->linebreak_ << be_idt << be_idt_nl
              << node->name () << "," << this->linebreak_ << be_nl
              << bt->name () << this->linebreak_ << be_uidt_nl
              << ">" << this->suffix_
              << be_uidt << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
              << "TAO_Seq_Var_Base_T<" << this->linebreak_ 
              << be_idt << be_idt_nl
              << node->name () << "," << this->linebreak_ << be_nl
              << bt->name () << this->linebreak_ << be_uidt_nl
              << ">" << this->suffix_
              << be_uidt << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
              << "TAO_Seq_Out_T<" << this->linebreak_ 
              << be_idt << be_idt_nl
              << node->name () << "," << this->linebreak_ << be_nl
              << node->name () << "_var," << this->linebreak_ << be_nl
              << bt->name () << this->linebreak_ << be_uidt_nl
              << ">" << this->suffix_
              << be_uidt << be_uidt << be_uidt;
        }

        break;
    }

  return 0;
}

int
be_visitor_tmplinst_cs::gen_base_class_tmplinst (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flat_name ());

  *os << be_nl << be_nl
      << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl;

  int status = node->gen_base_class_name (os, 
                                          this->linebreak_, 
                                          idl_global->root ());

  // Pass it to the base constructor.
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "gen_base_class_tmplinst - "
                         "codegen for base sequence class failed\n"),
                        -1);
    }

  *os << this->suffix_ << be_uidt << be_uidt;

  os->gen_endif ();

  return 0;
}

void
be_visitor_tmplinst_cs::gen_common_tmplinst (be_type *node,
                                             TAO_OutStream *os)
{
  idl_bool fixed = (node->size_type () == AST_Type::FIXED);

  *os << be_nl << be_nl
      << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
      << "TAO_" << (fixed ? "Fixed" : "Var") << "_Var_T<"
      << this->linebreak_ << be_idt << be_idt_nl
      << node->name () << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  *os << be_nl << be_nl
      << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
      << "TAO_Var_Base_T<" << this->linebreak_ << be_idt << be_idt_nl
      << node->name () << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  if (!fixed)
    {
      *os << be_nl<< be_nl
          << this->prefix_ << this->linebreak_ << be_idt << be_idt_nl
          << "TAO_Out_T<" << this->linebreak_ << be_idt << be_idt_nl
          << node->name () << "," << this->linebreak_ << be_nl
          << node->name () << "_var" << this->linebreak_ << be_uidt_nl
          << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;
    }
}

