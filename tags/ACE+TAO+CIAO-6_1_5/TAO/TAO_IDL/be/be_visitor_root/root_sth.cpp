
//=============================================================================
/**
 *  @file    root_sth.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the server template header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "root.h"

be_visitor_root_sth::be_visitor_root_sth (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_sth::~be_visitor_root_sth (void)
{
}

int
be_visitor_root_sth::visit_root (be_root *node)
{
  if (! be_global->gen_tie_classes ())
    {
      return 0;
    }

  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_sth::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_sth::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  (void) tao_cg->end_server_template_header ();

  return 0;
}

int
be_visitor_root_sth::visit_module (be_module *node)
{
  if (node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = tao_cg->server_template_header ();

  // Generate the skeleton class name.

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
               << "// " __FILE__ << ":" << __LINE__ << be_nl_2;

  // Now generate the class definition. The prefix POA_ is prepended to our
  // name only if we are the outermost module.
  *os << "namespace ";

  if (node->is_nested ())
    {
      // We are inside another module.
      *os << node->local_name () << be_nl;
    }
  else
    {
      // We are outermost module.
      *os << "POA_" << node->local_name () << be_nl;
    }

  *os << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_sth::"
                         "visit_module - "
                         "codegen for scope failed\n"),
                         -1);
    }

  *os << be_uidt_nl << "} // module " << node->name ();

  return 0;
}

int
be_visitor_root_sth::visit_interface (be_interface *node)
{
  if (node->imported () || node->is_local ())
    {
      return 0;
    }

  // Generate the TIE class.

  this->ctx_->node (node);
  be_visitor_interface_tie_sh visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_root_sth::"
                         "visit_interface - "
                         "codegen for TIE class failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root_sth::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_root_sth::init (void)
{
  /// First open the server-side file for writing
  int status =
    tao_cg->start_server_template_header (
      be_global->be_get_server_template_hdr_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_sth::init - ")
                         ACE_TEXT ("Error opening server ")
                         ACE_TEXT ("template header file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->server_template_header ());
  return 0;
}

