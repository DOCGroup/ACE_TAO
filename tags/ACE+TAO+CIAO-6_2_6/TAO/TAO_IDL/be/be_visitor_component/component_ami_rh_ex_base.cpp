
//=============================================================================
/**
 *  @file    component_ami_rh_ex_base.cpp
 *
 *  $Id$
 *
 *  Base class visitor generating code for AMI4CCM reply handler impl\
 *  visitors.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "component.h"

be_visitor_component_ami_rh_ex_base::be_visitor_component_ami_rh_ex_base (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    iface_ (0),
    port_ (0)
{
}

be_visitor_component_ami_rh_ex_base::~be_visitor_component_ami_rh_ex_base (
  void)
{
}

int
be_visitor_component_ami_rh_ex_base::post_process (be_decl *bd)
{
  // Has to be (1) an argument, (2) not an IN arg, and
  // (3) not the last INOUT or OUT arg.
  if (bd->node_type () == AST_Decl::NT_argument)
    {
      AST_Argument *arg =
        AST_Argument::narrow_from_decl (bd);

      if (arg->direction () != AST_Argument::dir_IN
          && ! this->last_inout_or_out_node (bd))
        {
          os_ << "," << be_nl;
        }
    }

  return 0;
}

void
be_visitor_component_ami_rh_ex_base::init (void)
{
  UTL_Scope *s = this->iface_->defined_in ();
  AST_Decl *scope = ScopeAsDecl (s);
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");

  ACE_CString base ("AMI4CCM_");
  base += this->iface_->original_local_name ()->get_string ();
  base += "ReplyHandler";

  this->base_class_name_ = scope->full_name ();
  this->base_class_name_ += smart_scope;
  this->base_class_name_ += "CCM_";
  this->base_class_name_ += base;

  this->class_name_ = base;
  this->class_name_ += '_';
  this->class_name_ +=
    this->port_->original_local_name ()->get_string ();
  this->class_name_ += "_i";
}

void
be_visitor_component_ami_rh_ex_base::gen_excep_op (
  const char *prefix,
  be_decl *node,
  bool for_defn)
{
  os_ << be_nl_2
      << (for_defn ? "" : "virtual ") << "void" << be_nl;

  if (for_defn)
    {
      os_ << this->class_name_ << "::";
    }

  os_ << prefix << node->local_name ()->get_string ()
      << "_excep (" << be_idt_nl
      << "::CCM_AMI::ExceptionHolder_ptr "
      << (for_defn ? "/* " : "") << "excep_holder"
      << (for_defn ? " */" : "") << ")"
      << (for_defn ? "" : ";") << be_uidt;

  this->gen_op_body ();
}

void
be_visitor_component_ami_rh_ex_base::gen_op_body (void)
{
}

int
be_visitor_component_ami_rh_ex_base::gen_attr_op (
  be_attribute *node,
  bool for_defn)
{
  os_ << be_nl_2
      << (for_defn ? "" : "virtual ") << "void" << be_nl;

  if (for_defn)
    {
      os_ << this->class_name_ << "::";
    }

  os_ << "get_" << node->local_name ()->get_string ()
      << " (" << be_idt_nl;

  be_argument arg (AST_Argument::dir_IN,
                   node->field_type (),
                   node->name ());

  be_visitor_args_arglist arg_visitor (this->ctx_);

  if (for_defn)
    {
      arg_visitor.unused (true);
    }

  if (arg_visitor.visit_argument (&arg) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_ex_base")
                         ACE_TEXT ("::gen_attr_op - ")
                         ACE_TEXT ("attr arg gen failed\n")),
                        -1);
    }

  os_ << ")" << (for_defn ? "" : ";") << be_uidt;

  arg.destroy ();

  this->gen_op_body ();

  this->gen_excep_op ("get_", node, for_defn);

  if (! node->readonly ())
    {
      os_ << be_nl_2
          << (for_defn ? "" : "virtual ") << "void" << be_nl;

      if (for_defn)
        {
          os_ << this->class_name_ << "::";
        }

      os_ << "set_" << node->local_name ()->get_string ()
          << " (void)" << (for_defn ? "" : ";");

      this->gen_op_body ();

      this->gen_excep_op ("set_", node, for_defn);
    }

  return 0;
}

// ======================================================

Exec_ReplyHandler_Op_Attr_Generator::Exec_ReplyHandler_Op_Attr_Generator (
    be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Exec_ReplyHandler_Op_Attr_Generator::emit (
  be_interface * /* derived_interface */,
  TAO_OutStream * /* os */,
  be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}

