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

#include "be_visitor_tmplinst_cs.h"
#include "be_visitor_arg_tmplinst.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_valuetype.h"
#include "be_interface_fwd.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_array.h"
#include "be_enum.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_structure_fwd.h"
#include "be_field.h"
#include "be_union.h"
#include "be_union_fwd.h"
#include "be_union_branch.h"
#include "be_typedef.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"

be_visitor_tmplinst_cs::be_visitor_tmplinst_cs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    mode_ (TMPL_CLASS),
    prefix_ (ACE::strnew ("  template class")),
    suffix_ (ACE::strnew (";")),
    linebreak_ (ACE::strnew (""))
{
}

be_visitor_tmplinst_cs::~be_visitor_tmplinst_cs (void)
{
  delete [] this->prefix_;
  this->prefix_ = 0;
  delete [] this->suffix_;
  this->suffix_ = 0;
  delete [] this->linebreak_;
  this->linebreak_ = 0;
}

int
be_visitor_tmplinst_cs::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_tmplinst_cs::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_tmplinst_cs::visit_interface (be_interface *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

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

  // @@@ (JP) This condition may change or go away once we 
  // regenerate the ORB hand-crafted code.
  if (!node->is_defined ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Objref_Traits<" << node->name ()
          << ">" << this->suffix_;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_interface_fwd (be_interface_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_interface *fd = 
    be_interface::narrow_from_decl (node->full_definition ());

  // The logic in visit_interface() should handle what gets generated
  // and what doesn't.
  if (this->visit_interface (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_interface_fwd - code generation failed\n"),
                        -1);
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

  // @@@ (JP) This condition may change or go away once we 
  // regenerate the ORB hand-crafted code.
  if (!node->is_defined ())
    {
      *os << be_nl << be_nl
          << this->prefix_ << " TAO::Value_Traits<" << node->name ()
          << ">" << this->suffix_;
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_valuetype *fd = 
    be_valuetype::narrow_from_decl (node->full_definition ());

  // The logic in visit_valuetype() should handle what gets generated
  // and what doesn't.
  int status = this->visit_valuetype (fd);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_valuetype_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int 
be_visitor_tmplinst_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
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

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_enum (be_enum *node)
{
  if (this->this_mode_generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flat_name (), "arg_traits_tmplinst");

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::Arg_Traits<" << node->name ()
      << ">" << this->suffix_;

  *os << be_nl << be_nl
      << this->prefix_ << " TAO::Basic_Arg_Traits_T<" << node->name ()
      << ">" << this->suffix_;

  os->gen_endif ();

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
              << node->name () << "_out" << this->linebreak_;
        }

      *os << be_uidt_nl
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

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_structure_fwd (be_structure_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_structure *fd = be_structure::narrow_from_decl (node->full_definition ());

  if (this->visit_structure (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_structure_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_cs::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

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
              << node->name () << "_out" << this->linebreak_;
        }

      *os << be_uidt_nl
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

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_cs::visit_union_fwd (be_union_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_union *fd = be_union::narrow_from_decl (node->full_definition ());

  if (this->visit_union (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_union_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"), 
                        -1);
    }

  return 0;
}

int 
be_visitor_tmplinst_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
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
be_visitor_tmplinst_cs::switch_mode (void)
{
  this->mode_ = PRAGMA_INST;
  delete [] this->prefix_;
  this->prefix_ = ACE::strnew ("# pragma instantiate");
  delete [] this->suffix_;
  this->suffix_ = ACE::strnew ("");
  delete [] this->linebreak_;
  this->linebreak_ = ACE::strnew (" \\");
}

