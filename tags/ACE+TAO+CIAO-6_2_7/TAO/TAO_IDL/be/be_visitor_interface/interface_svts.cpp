
//=============================================================================
/**
 *  @file    interface_svts.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the
 *  CIAO template servant source file.
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_svts::be_visitor_interface_svts (
  be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_svts::~be_visitor_interface_svts (void)
{
}

int
be_visitor_interface_svts::visit_interface (be_interface *node)
{
  be_visitor_facet_svts facet_svts (this->ctx_);

  if (facet_svts.visit_interface (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svts::")
                         ACE_TEXT ("visit_provides - ")
                         ACE_TEXT ("facet visitor failed\n")),
                        -1);
    }
  return 0;
}

int
be_visitor_interface_svts::visit_operation (be_operation *node)
{
  be_visitor_operation_svs v (this->ctx_);
  v.scope (be_interface::narrow_from_decl (
             this->ctx_->scope ()->decl ()));

  return v.visit_operation (node);
}

int
be_visitor_interface_svts::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);

  v.for_facets (true);
  v.op_scope (be_interface::narrow_from_decl (
                this->ctx_->scope ()->decl ()));

  return v.visit_attribute (node);
}


