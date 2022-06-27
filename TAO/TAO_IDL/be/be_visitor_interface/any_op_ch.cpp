
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Any operators for an Interface in the client
 *  header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_any_op_ch::be_visitor_interface_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_any_op_ch::~be_visitor_interface_any_op_ch ()
{
}

int
be_visitor_interface_any_op_ch::visit_interface (be_interface *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ()
      || (node->is_local () && !be_global->gen_local_iface_anyops ()))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl_2;

  TAO_INSERT_COMMENT (os);

  *os << be_nl_2;

  *os << be_global->anyops_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << "_ptr); // copying" << be_nl;
  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << "_ptr *); // non-copying" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << "_ptr &);";

  *os << be_global->anyops_versioning_end () << be_nl;

  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_any_op_ch::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }


  node->cli_hdr_any_op_gen (true);
  return 0;
}

int
be_visitor_interface_any_op_ch::visit_component (
  be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_any_op_ch::visit_connector (
  be_connector *node)
{
  return this->visit_interface (node);
}
