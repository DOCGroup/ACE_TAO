/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    component.h
 *
 *  $Id$
 *
 *  Concrete visitor for components
 *  This one provides the generic visitor for the be_component node.
 *
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

  ~be_visitor_component (void);

  /// This is the only type of declaration a component may contain.
  virtual int visit_attribute (be_attribute *node);

  /// Operations are created by the back end for 'provides', 'uses', 'emits',
  /// 'publishes' and 'consumes' declarations.
  virtual int visit_operation (be_operation *node);

  /// Structs and sequences are created by the back end for 'uses multiple'
  /// declarations.
  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);

  /// Overridden to pick up attribute declarations from a porrttype.
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_mirror_port (be_mirror_port *node);
};

#endif /* _BE_VISITOR_MODULE_MODULE_H_ */
