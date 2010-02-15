/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_si.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the server inline
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SI_H_
#define _BE_COMPONENT_COMPONENT_SI_H_

/**
 * @class be_visitor_component_si
 *
 * @brief be_visitor_component_sh
 *
 * This is a concrete visitor to generate the server header for component.
 */
class be_visitor_component_si : public be_visitor_component
{
public:
  be_visitor_component_si (be_visitor_context *ctx);
  ~be_visitor_component_si (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_COMPONENT_COMPONENT_SI_H_ */
