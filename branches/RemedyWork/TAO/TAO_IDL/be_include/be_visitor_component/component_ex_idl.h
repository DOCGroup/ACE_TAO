/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ex_idl.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  Provides code generation for components in the CIAO executor IDL
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_EX_IDL_H_
#define _BE_COMPONENT_COMPONENT_EX_IDL_H_

/**
 * @class be_visitor_component_ex_idl
 *
 * @brief be_visitor_component_ex_idl
 *
 * This is a concrete visitor to generate the
 * CIAO executor IDL for component.
 */
class be_visitor_component_ex_idl : public be_visitor_decl
{
public:
  be_visitor_component_ex_idl (be_visitor_context *ctx);

  ~be_visitor_component_ex_idl (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);

private:
  TAO_OutStream &os_;
};

#endif /* _BE_COMPONENT_COMPONENT_EX_IDL_H_ */
