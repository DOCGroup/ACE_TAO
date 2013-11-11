
//=============================================================================
/**
 *  @file    interface_svth.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the
 *  CIAO template servant header file.
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_svth::be_visitor_interface_svth (
  be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_svth::~be_visitor_interface_svth (void)
{
}

int
be_visitor_interface_svth::visit_interface (be_interface *node)
{
  be_visitor_facet_svth facet_svth(this->ctx_);

  if (facet_svth.visit_interface (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svth::")
                         ACE_TEXT ("visit_provides - ")
                         ACE_TEXT ("facet visitor failed\n")),
                        -1);
    }
  return 0;
}

int
be_visitor_interface_svth::visit_operation (be_operation *node)
{
  be_visitor_operation_ch v (this->ctx_);
  return v.visit_operation (node);
}

int
be_visitor_interface_svth::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}


