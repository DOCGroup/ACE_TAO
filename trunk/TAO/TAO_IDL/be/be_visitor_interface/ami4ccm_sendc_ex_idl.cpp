
//=============================================================================
/**
 *  @file    ami4ccm_sendc_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM Interfaces in the executor IDL
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "interface.h"

be_visitor_ami4ccm_sendc_ex_idl::be_visitor_ami4ccm_sendc_ex_idl (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    os_ (*ctx->stream ())
{
}

be_visitor_ami4ccm_sendc_ex_idl::~be_visitor_ami4ccm_sendc_ex_idl (void)
{
}

int
be_visitor_ami4ccm_sendc_ex_idl::visit_interface (be_interface *node)
{
  this->iface_ = node;

  os_ << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os_ << be_nl_2
      << "local interface AMI4CCM_"
      << node->original_local_name () << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_ami4ccm_sendc_ex_idl")
                         ACE_TEXT ("::visit_interface - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  AST_Interface **inh_flat = node->inherits_flat ();

  for (long i = 0; i < node->n_inherits_flat (); ++i)
    {
      be_interface *ancestor =
        be_interface::narrow_from_decl (inh_flat[i]);

      if (this->visit_scope (ancestor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_ami4ccm_sendc_ex_idl")
                             ACE_TEXT ("::visit_interface - visit ")
                             ACE_TEXT ("ancestor scope failed\n")),
                            -1);
        }
    }

  os_ << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_ami4ccm_sendc_ex_idl::visit_operation (be_operation *node)
{
  if (node->flags () == AST_Operation::OP_oneway)
    {
      // We do nothing for oneways!
      return 0;
    }

  if (!node->is_sendc_ami ())
    {
      return 0;
    }

  os_ << be_nl
      << "void " << node->original_local_name ()
      << " (" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_ami4ccm_sendc_ex_idl")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  os_ << ");" << be_uidt;

  return 0;
}

int
be_visitor_ami4ccm_sendc_ex_idl::visit_argument (be_argument *node)
{
  if (node->direction () != AST_Argument::dir_IN)
    {
      return 0;
    }

  /// AMI4CCM uses a different reply handler type, so we just
  /// replace the original handler parameter (which is always the
  /// first one) type.
  if (this->elem_number () == 1)
    {
      os_ << be_nl
          << "in AMI4CCM_" << this->iface_->original_local_name ()
          << "ReplyHandler";
    }
  else
    {
      be_type *t =
        be_type::narrow_from_decl (node->field_type ());

      os_ << be_nl
          << "in ";

      os_ << IdentifierHelper::type_name (t, this);
    }

  os_ << " " << node->original_local_name ();

  return 0;
}

int
be_visitor_ami4ccm_sendc_ex_idl::visit_string (be_string *node)
{
  os_ << (node->width () > 1 ? "w" : "") << "string";

  ACE_CDR::ULong const bound = node->max_size ()->ev ()->u.ulval;

  if (bound > 0)
    {
      os_ << "<" << bound << ">";
    }

  return 0;
}

int
be_visitor_ami4ccm_sendc_ex_idl::visit_sequence (be_sequence *node)
{
  // Keep output statements separate because of side effects.
  os_ << "sequence<";
  os_ << IdentifierHelper::type_name (node->base_type (), this);

  if (!node->unbounded ())
    {
      os_ << ", " << node->max_size ()->ev ()->u.ulval;
    }

  os_ << "> ";

  return 0;
}

int
be_visitor_ami4ccm_sendc_ex_idl::pre_process (be_decl *node)
{
  be_argument *arg = be_argument::narrow_from_decl (node);

  if (arg == 0)
    {
      return 0;
    }

  if (arg->direction () != AST_Argument::dir_IN)
    {
      return 0;
    }

  if (this->elem_number () == 1)
    {
      return 0;
    }

  os_ << ",";

  return 0;
}

