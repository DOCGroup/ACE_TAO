/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_svh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the servant header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SVH_H_
#define _BE_COMPONENT_COMPONENT_SVH_H_

/**
 * @class be_visitor_component_svh
 *
 * @brief be_visitor_component_svh
 *
 * This is a concrete visitor to generate the servant header for component.
 */
class be_visitor_component_svh : public be_visitor_component
{
public:
  be_visitor_component_svh (be_visitor_context *ctx);

  ~be_visitor_component_svh (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);

private:
  void gen_entrypoint (be_component *node);

private:
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_COMPONENT_COMPONENT_SVH_H_ */

