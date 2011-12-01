/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_svth.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the templated servant header
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SVTH_H_
#define _BE_COMPONENT_COMPONENT_SVTH_H_

/**
 * @class be_visitor_component_svth
 *
 * @brief be_visitor_component_svth
 *
 * This is a concrete visitor to generate the servant header for component.
 */
class be_visitor_component_svth : public be_visitor_component
{
public:
  be_visitor_component_svth (be_visitor_context *ctx);

  ~be_visitor_component_svth (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);

private:
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_COMPONENT_COMPONENT_SVTH_H_ */

