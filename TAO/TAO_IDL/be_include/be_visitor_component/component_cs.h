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
  be_visitor_component_cs (be_visitor_context *ctx);
  ~be_visitor_component_cs (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_COMPONENT_COMPONENT_CS_H_ */
