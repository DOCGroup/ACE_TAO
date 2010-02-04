/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This one provides code generation for components in the client stub.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_CS_H_
#define _BE_COMPONENT_COMPONENT_CS_H_

/**
 * @class be_visitor_component_cs
 *
 * @brief be_visitor_component_cs
 *
 * This is a concrete visitor to generate the client stub for component.
 */
class be_visitor_component_cs : public be_visitor_component
{
public:
  /// constructor
  be_visitor_component_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_component_cs (void);

  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_CS_H_ */
