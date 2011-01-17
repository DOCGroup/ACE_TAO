/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface_sh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the server header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_INTERFACE_INTERFACE_SH_H_
#define _BE_INTERFACE_INTERFACE_SH_H_

/**
 * @class be_visitor_interface_sh
 *
 * @brief be_visitor_interface_sh
 *
 * This is a concrete visitor to generate the server header for interface
 */
class be_visitor_interface_sh : public be_visitor_interface
{
public:
  be_visitor_interface_sh (be_visitor_context *ctx);
  ~be_visitor_interface_sh (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);

  /// Helper to generate the declarations for the operations
  /// of any abstract parents we may have.
  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);

protected:
  virtual void this_method (be_interface *node);
  virtual int generate_amh_classes (be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SH_H_ */
