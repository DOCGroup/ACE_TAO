/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    component.h
 *
 *  Concrete visitor for components
 *  This one provides the generic visitor for the be_component node.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_COMPONENT_COMPONENT_H_
#define _BE_VISITOR_COMPONENT_COMPONENT_H_

/**
 * @class be_visitor_component
 *
 * @brief be_visitor_component
 *
 * This is a concrete visitor for component
 * that abstracts all common tasks.
 */
class be_visitor_component : public be_visitor_interface
{
public:
  be_visitor_component (be_visitor_context *ctx);

  ~be_visitor_component ();

  /// This is the only type of declaration a component may contain.
  int visit_attribute (be_attribute *node) override;

  /// Operations are created by the back end for 'provides', 'uses', 'emits',
  /// 'publishes' and 'consumes' declarations.
  int visit_operation (be_operation *node) override;

  /// Structs and sequences are created by the back end for 'uses multiple'
  /// declarations.
  int visit_structure (be_structure *node) override;
  int visit_typedef (be_typedef *node) override;

  /// Overridden to pick up attribute declarations from a porrttype.
  int visit_extended_port (be_extended_port *node) override;
  int visit_mirror_port (be_mirror_port *node) override;
};

#endif /* _BE_VISITOR_MODULE_MODULE_H_ */
