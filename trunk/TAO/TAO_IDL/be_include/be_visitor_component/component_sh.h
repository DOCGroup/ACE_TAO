/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_sh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the server header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_SH_H_
#define _BE_COMPONENT_COMPONENT_SH_H_

/**
 * @class be_visitor_component_sh
 *
 * @brief be_visitor_component_sh
 *
 * This is a concrete visitor to generate the server header for component.
 */
class be_visitor_component_sh : public be_visitor_component
{
public:
  /// constructor
  be_visitor_component_sh (be_visitor_context *ctx);
  
  /// destructor
  ~be_visitor_component_sh (void);
  
  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);
protected:
  virtual int generate_amh_classes (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_SH_H_ */
