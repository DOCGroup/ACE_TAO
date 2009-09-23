/**
 *  @file   be_visitor_iface_inst.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_visitor_iface_inst.h"

be_visitor_iface_inst::be_visitor_iface_inst (
      be_visitor_context *ctx,
      be_interface *instance,
      AST_Template_Common::T_ARGLIST *args)
  : be_visitor_scope (ctx),
    instance_ (instance),
    template_args_ (args)
{
}

be_visitor_iface_inst::~be_visitor_iface_inst (void)
{
}

int
be_visitor_iface_inst::visit_argument (be_argument *)
{
  return 0;
}

int
be_visitor_iface_inst::visit_operation (be_operation *)
{
  return 0;
}

int
be_visitor_iface_inst::visit_attribute (be_attribute *)
{
  return 0;
}

int
be_visitor_iface_inst::visit_typedef (be_typedef *)
{
  return 0;
}

int
be_visitor_iface_inst::visit_exception (be_exception *)
{
  return 0;
}


