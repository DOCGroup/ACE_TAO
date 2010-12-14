/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This one provides code generation for interfaces in the client inline.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_INTERFACE_INTERFACE_CI_H_
#define _BE_INTERFACE_INTERFACE_CI_H_

/**
 * @class be_visitor_interface_ci
 *
 * @brief be_visitor_interface_ci
 *
 * This is a concrete visitor to generate the client inline for interface
 */
class be_visitor_interface_ci : public be_visitor_interface
{
public:
  be_visitor_interface_ci (be_visitor_context *ctx);
  ~be_visitor_interface_ci (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_INTERFACE_INTERFACE_CI_H_ */
