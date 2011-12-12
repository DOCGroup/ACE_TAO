/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_svts.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the templated servant source
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SVTS_H_
#define _BE_COMPONENT_COMPONENT_SVTS_H_

/**
 * @class be_visitor_component_svts
 *
 * @brief be_visitor_component_svts
 *
 * This is a concrete visitor to generate the servant
 * source for component.
 */
class be_visitor_component_svts : public be_visitor_component
{
public:
  be_visitor_component_svts (be_visitor_context *ctx);

  ~be_visitor_component_svts (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);

private:
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_COMPONENT_COMPONENT_SVTS_H_ */

