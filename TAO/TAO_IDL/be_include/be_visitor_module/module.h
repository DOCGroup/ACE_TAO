/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    module.h
 *
 *  Concrete visitor for the Module class
 *  This one provides the generic visitor for the Module node.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MODULE_MODULE_H_
#define _BE_VISITOR_MODULE_MODULE_H_

/**
 * @class be_visitor_module
 *
 * @brief be_visitor_module
 *
 * This is a concrete visitor for module that abstracts all common tasks.
 */
class be_visitor_module : public be_visitor_scope
{

public:
  be_visitor_module (be_visitor_context *ctx);

  ~be_visitor_module ();

  int visit_constant (be_constant *node) override;
  int visit_enum (be_enum *node) override;
  int visit_native (be_native *node) override;
  int visit_exception (be_exception *node) override;
  int visit_interface (be_interface *node) override;
  int visit_interface_fwd (be_interface_fwd *node) override;
  int visit_valuebox (be_valuebox *node) override;
  int visit_valuetype (be_valuetype *node) override;
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;
  int visit_eventtype (be_eventtype *node) override;
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;
  int visit_component (be_component *node) override;
  int visit_component_fwd (be_component_fwd *node) override;
  int visit_home (be_home *node) override;
  int visit_connector (be_connector *node) override;
  int visit_module (be_module *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
  int visit_typedef (be_typedef *node) override;
};

#endif /* _BE_VISITOR_MODULE_MODULE_H_ */
