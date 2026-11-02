
//=============================================================================
/**
 *  @file    component_ami_rh_exs.cpp
 *
 *  Visitor generating code for AMI4CCM reply handler impl source
 *  files.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "component.h"

#include <memory>
#include <string>

be_visitor_component_ami_rh_exs::be_visitor_component_ami_rh_exs (
      be_visitor_context *ctx)
  : be_visitor_component_ami_rh_ex_base (ctx)
{
}

be_visitor_component_ami_rh_exs::~be_visitor_component_ami_rh_exs () = default;

int
be_visitor_component_ami_rh_exs::visit_uses (be_uses *node)
{
  this->port_ = node;

  this->iface_ =
    dynamic_cast<be_interface*> (node->uses_type ());

  this->init ();

  os_ << be_nl_2
      << this->class_name_ << "::"
      << this->class_name_ << " ()" << be_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << this->class_name_ << "::~"
      << this->class_name_ << " ()" << be_nl
      << "{" << be_nl
      << "}";

  std::string const port_pfix = this->ctx_->port_prefix ();
  this->ctx_->port_prefix () = "";

  /// This overload of traverse_inheritance_graph() used here
  /// doesn't automatically prime the queues.
  this->iface_->get_insert_queue ().clear ();
  this->iface_->get_del_queue ().clear ();
  this->iface_->get_insert_queue ().push_back (this->iface_);

  Exec_ReplyHandler_Op_Attr_Generator op_attr_gen (this);

  int const status =
    this->iface_->traverse_inheritance_graph (op_attr_gen,
                                              &os_,
                                              false,
                                              false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exs")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("traverse_inheritance_graph() on ")
                         ACE_TEXT ("callback interface failed\n")),
                        -1);
    }

  this->ctx_->port_prefix (port_pfix);
  return 0;
}

int
be_visitor_component_ami_rh_exs::visit_operation (
  be_operation *node)
{
  /// Skip these for the reply handler.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  AST_Decl *d =
    ScopeAsDecl (node->defined_in ());

  /// We end up here also from the visit_scope() call on the
  /// connector. We want to skip the CCM-related operations
  /// that were added to the connector since it's a component.
  /// We want only the facet interface operations.
  if (d->node_type () != AST_Decl::NT_interface)
    {
      return  0;
    }

  os_ << be_nl_2
      << "void" << be_nl
      << this->class_name_ << "::"
      << node->local_name ()->get_string ()
      << " (";

  int const count =
    node->count_arguments_with_direction (
      AST_Argument::dir_INOUT | AST_Argument::dir_OUT);

  bool const vrt = node->void_return_type ();

  if (count == 0 && vrt)
    {
      os_ << ")";
    }
  else
    {
      os_ << be_idt_nl;
    }

  if (!vrt)
    {
      // ACE_NEW_RETURN requires raw output pointers; adopt each allocation immediately.
      Identifier *id = nullptr;

      ACE_NEW_RETURN (id,
                      Identifier ("ami_return_val"),
                      -1);
      std::unique_ptr<Identifier> id_owner (id);

      UTL_ScopedName *tmp = nullptr;

      ACE_NEW_RETURN (tmp,
                      UTL_ScopedName (id_owner.get (),
                                      nullptr),
                      -1);
      id_owner.release ();
      FE_Utils::Scoped_Name_Owner tmp_owner (tmp);

      FE_Utils::Scoped_Name_Owner argument_name (
        static_cast<UTL_ScopedName *> (node->name ()->copy ()));
      argument_name->nconc (tmp_owner.get ());
      tmp_owner.release ();

      // Create the argument.
      be_argument *arg = nullptr;
      ACE_NEW_RETURN (arg,
                      be_argument (AST_Argument::dir_OUT,
                                  node->return_type (),
                                  argument_name.get ()),
                      -1);
      std::unique_ptr<be_argument, BE_AST_Decl_Deleter> const arg_owner (arg);

      arg->set_defined_in (node);
      arg->set_name (argument_name.get ());
      argument_name.release ();

      if (this->visit_argument (arg) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              ACE_TEXT ("be_visitor_component")
                              ACE_TEXT ("_ami_rh_exh")
                              ACE_TEXT ("::visit_operation - ")
                              ACE_TEXT ("return type arg")
                              ACE_TEXT (" gen failed\n")),
                            -1);
        }

      if (count != 0)
        {
          os_ << "," << be_nl;
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exs")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  if (count != 0 || ! vrt)
    {
      os_ << ")" << be_uidt;
    }

  this->gen_op_body ();

  this->gen_excep_op ("", node, true);

  return 0;
}

int
be_visitor_component_ami_rh_exs::visit_attribute (
  be_attribute *node)
{
  return this->gen_attr_op (node, true);
}

int
be_visitor_component_ami_rh_exs::visit_argument (
  be_argument *node)
{
  if (node->direction () == AST_Argument::dir_IN)
    {
      return 0;
    }

  be_visitor_args_arglist arg_visitor (this->ctx_);
  arg_visitor.set_fixed_direction (AST_Argument::dir_IN);
  arg_visitor.unused (true);

  if (arg_visitor.visit_argument (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_ami_rh_exs")
                         ACE_TEXT ("::visit_argument - ")
                         ACE_TEXT ("be_visitor_args_arglist failed\n")),
                        -1);
    }

  return 0;
}

void
be_visitor_component_ami_rh_exs::gen_op_body ()
{
  os_ << be_nl
      << "{" << be_idt_nl
      << this->your_code_here_ << be_uidt_nl
      << "}";
}

