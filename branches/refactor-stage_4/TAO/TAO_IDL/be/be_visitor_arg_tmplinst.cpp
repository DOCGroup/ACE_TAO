//=============================================================================
/**
*  @file   be_visitor_arg_tmplinst.cpp
*
*  $Id$
*
*  This visitor generates explicit template instantiations, guarded so they
*  are seen only by compilers whose platforms require explicit template
*  instantiation, for the individual argument template helper classes.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

#include "be_visitor_arg_tmplinst.h"
#include "be_visitor_context.h"
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
#include "be_helper.h"

be_visitor_arg_tmplinst::be_visitor_arg_tmplinst (
    be_visitor_context *ctx,
    be_visitor_tmplinst_cs::Mode mode,
    char * prefix,
    char * suffix,
    char * linebreak,
    const char * S
  )
  : be_visitor_decl (ctx),
    mode_ (mode),
    prefix_ (prefix),
    suffix_ (suffix),
    linebreak_ (linebreak),
    S_ (S)
{
}

be_visitor_arg_tmplinst::~be_visitor_arg_tmplinst (void)
{
}

int
be_visitor_arg_tmplinst::visit_interface (be_interface *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl
      << "TAO::";

  this->gen_direction (os);

  *os << "_Object_" << this->S_ << "Argument_T<" << this->linebreak_ 
      << be_idt << be_idt_nl
      << node->name () << "_ptr";

  switch (this->dir_)
    {
      case _tao_OUT:
        *os << "," << this->linebreak_ << be_nl
            << node->name () << "_out";
        break;
      case _tao_RET:
        *os << "," << this->linebreak_ << be_nl
            << node->name () << "_var";
        break;
      default:
        break;
    }

  *os << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_interface_fwd (be_interface_fwd *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  be_interface *fd = 
    be_interface::narrow_from_decl (node->full_definition ());

  if (this->visit_interface (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_interface_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_valuetype (be_valuetype *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl 
      << "TAO::";

  this->gen_direction (os);

  *os << "_Object_" << this->S_ << "Argument_T<" << this->linebreak_ 
      << be_idt << be_idt_nl
      << node->name () << " *";

  switch (this->dir_)
    {
      case _tao_OUT:
        *os << "," << this->linebreak_ << be_nl
            << node->name () << "_out";
        break;
      case _tao_RET:
        *os << "," << this->linebreak_ << be_nl
            << node->name () << "_var";
        break;
      default:
        break;
    }

  *os << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  be_valuetype *fd = 
    be_valuetype::narrow_from_decl (node->full_definition ());

  if (this->visit_valuetype (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_valuetype_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int 
be_visitor_arg_tmplinst::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_arg_tmplinst::visit_sequence (be_sequence *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl 
      << "TAO::";

  this->gen_direction (os);

  *os << "_Var_Size_" << this->S_ << "Argument_T<" << this->linebreak_ 
      << be_idt << be_idt_nl
      << node->name ();

  switch (this->dir_)
    {
      case _tao_OUT:
        *os << "," << this->linebreak_ << be_nl
            << node->name () << "_out";
        break;
      case _tao_RET:
        *os << "," << this->linebreak_ << be_nl
            << node->name () << "_var";
        break;
      default:
        break;
    }

  *os << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_string (be_string *node)
{
  if (this->this_mode_and_dir_generated (node))
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
      this->this_mode_and_dir_generated (node, I_TRUE);
      return 0;
    }

  idl_bool wide = (node->width () != 1);

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl 
      << "TAO::";

  this->gen_direction (os);

  *os << "_BD_String_" << this->S_ << "Argument_T<" << this->linebreak_ 
      << be_idt << be_idt_nl
      << "CORBA::" << (wide ? "W" : "") << "Char";

  switch (this->dir_)
    {
      case _tao_OUT:
        *os << "," << this->linebreak_ << be_nl
            << "CORBA::" << (wide ? "W" : "") << "String_out";
        break;
      case _tao_RET:
        *os << "," << this->linebreak_ << be_nl
            << "CORBA::" << (wide ? "W" : "") << "String_var";
        break;
      default:
        break;
    }

  *os << "," << this->linebreak_ << be_nl
      << "ACE_InputCDR::to_" << (wide ? "w" : "") << "char," 
      << this->linebreak_ << be_nl
      << "ACE_OutputCDR::from_" << (wide ? "w" : "") << "char,"
      << this->linebreak_ << be_nl
      << bound << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_array (be_array *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  idl_bool fixed = (node->size_type () == AST_Type::FIXED);

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl 
      << "TAO::";

  this->gen_direction (os);

  *os << "_" << (fixed ? "Fixed" : "Var") << "_Array_" 
      << this-> S_ << "Argument_T<" 
      << this->linebreak_ << be_idt << be_idt_nl
      << node->name () << "," << this->linebreak_ << be_nl;

  switch (this->dir_)
    {
      case _tao_IN:
        *os << node->name () << "_slice," << this->linebreak_ << be_nl;
        break;
      case _tao_OUT:
        if (!fixed)
          {
            *os << node->name () << "_slice," << this->linebreak_ << be_nl
                << node->name () << "_var," << this->linebreak_ << be_nl
                << node->name () << "_out," << this->linebreak_ << be_nl;
          }

        break;
      case _tao_RET:
        *os << node->name () << "_slice," << this->linebreak_ << be_nl
            << node->name () << "_var," << this->linebreak_ << be_nl;
        break;
      default:
        break;
    }

  *os << node->name () << "_forany" << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_enum (be_enum *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl 
      << "TAO::";

  this->gen_direction (os);

  *os << "_Basic_" << this->S_ << "Argument_T<" << be_idt << be_idt_nl
      << node->name () << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_structure (be_structure *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  idl_bool fixed = (node->size_type () == AST_Type::FIXED);

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl 
      << "TAO::";

  this->gen_direction (os);

  *os << "_" << (fixed ? "Fixed" : "Var") << "_Size_" 
      << this->S_ << "Argument_T<" 
      << be_idt << be_idt_nl
      << node->name ();
  
  if (!fixed)
    {   
      switch (this->dir_)
        {
          case _tao_OUT:
            *os << "," << this->linebreak_ << be_nl
                << node->name () << "_out";
            break;
          case _tao_RET:
            *os << "," << this->linebreak_ << be_nl
                << node->name () << "_var";
            break;
          default:
            break;
        }
    }

  *os << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_structure_fwd (be_structure_fwd *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  be_structure *fd = be_structure::narrow_from_decl (node->full_definition ());

  if (this->visit_structure (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_structure_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_tmplinst::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

int 
be_visitor_arg_tmplinst::visit_union (be_union *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  idl_bool fixed = (node->size_type () == AST_Type::FIXED);

  *os << be_nl << be_nl
      << this->prefix_ << be_idt << be_idt_nl 
      << "TAO::";

  this->gen_direction (os);

  *os << "_" << (fixed ? "Fixed" : "Var") << "_Size_" 
      << this-> S_ << "Argument_T<" 
      << be_idt << be_idt_nl
      << node->name ();
  
  if (!fixed)
    {   
      switch (this->dir_)
        {
          case _tao_OUT:
            *os << "," << this->linebreak_ << be_nl
                << node->name () << "_out";
            break;
          case _tao_RET:
            *os << "," << this->linebreak_ << be_nl
                << node->name () << "_var";
            break;
          default:
            break;
        }
    }

  *os << this->linebreak_ << be_uidt_nl
      << ">" << this->suffix_ << be_uidt << be_uidt << be_uidt;

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_union_fwd (be_union_fwd *node)
{
  if (this->this_mode_and_dir_generated (node))
    {
      return 0;
    }

  be_union *fd = be_union::narrow_from_decl (node->full_definition ());

  if (this->visit_union (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_union_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_and_dir_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_tmplinst::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"), 
                        -1);
    }

  return 0;
}

int 
be_visitor_arg_tmplinst::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

void
be_visitor_arg_tmplinst::direction (void)
{
  this->dir_ = _tao_RET;
}

void
be_visitor_arg_tmplinst::direction (AST_Argument::Direction dir)
{
  switch (dir)
    {
      case AST_Argument::dir_IN:
        this->dir_ = _tao_IN;
        break;
      case AST_Argument::dir_INOUT:
        this->dir_ = _tao_INOUT;
        break;
      case AST_Argument::dir_OUT:
        this->dir_ = _tao_OUT;
        break;
      default:
        break;
    }
}

idl_bool
be_visitor_arg_tmplinst::this_mode_and_dir_generated (be_decl *node) const
{
  switch (this->mode_)
    {
      case be_visitor_tmplinst_cs::TMPL_CLASS:
        switch (this->dir_)
          {
            case _tao_IN:
              return node->cli_inarg_tmpl_class_gen ();
            case _tao_INOUT:
              return node->cli_inoutarg_tmpl_class_gen ();
            case _tao_OUT:
              return node->cli_outarg_tmpl_class_gen ();
            case _tao_RET:
              return node->cli_retarg_tmpl_class_gen ();
            default:
              return I_FALSE;
          }
      case be_visitor_tmplinst_cs::PRAGMA_INST:
        switch (this->dir_)
          {
            case _tao_IN:
              return node->cli_inarg_pragma_inst_gen ();
            case _tao_INOUT:
              return node->cli_inoutarg_pragma_inst_gen ();
            case _tao_OUT:
              return node->cli_outarg_pragma_inst_gen ();
            case _tao_RET:
              return node->cli_retarg_pragma_inst_gen ();
            default:
              return I_FALSE;
          }
      default:
        return I_FALSE;
    }
}

void
be_visitor_arg_tmplinst::this_mode_and_dir_generated (be_decl *node,
                                                      idl_bool val)
{
  switch (this->mode_)
    {
      case be_visitor_tmplinst_cs::TMPL_CLASS:
        switch (this->dir_)
          {
            case _tao_IN:
              node->cli_inarg_tmpl_class_gen (val);
              break;
            case _tao_INOUT:
              node->cli_inoutarg_tmpl_class_gen (val);
              break;
            case _tao_OUT:
              node->cli_outarg_tmpl_class_gen (val);
              break;
            case _tao_RET:
              node->cli_retarg_tmpl_class_gen (val);
              break;
            default:
              break;
          }
        break;
      case be_visitor_tmplinst_cs::PRAGMA_INST:
        switch (this->dir_)
          {
            case _tao_IN:
              node->cli_inarg_pragma_inst_gen (val);
              break;
            case _tao_INOUT:
              node->cli_inoutarg_pragma_inst_gen (val);
              break;
            case _tao_OUT:
              node->cli_outarg_pragma_inst_gen (val);
              break;
            case _tao_RET:
              node->cli_retarg_pragma_inst_gen (val);
              break;
            default:
              break;
          }
        break;
      default:
        break;
    }
}

void
be_visitor_arg_tmplinst::gen_direction (TAO_OutStream *os)
{
  switch (this->dir_)
    {
      case _tao_IN:
        *os << "In";
        break;
      case _tao_INOUT:
        *os << "Inout";
        break;
      case _tao_OUT:
        *os << "Out";
        break;
      case _tao_RET:
        *os << "Ret";
        break;
      default:
        break;
    }
}

