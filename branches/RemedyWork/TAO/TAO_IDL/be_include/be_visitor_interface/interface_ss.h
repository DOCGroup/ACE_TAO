// -*- C++ -*-


//=============================================================================
/**
 *  @file    interface_ss.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the server skeleton
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_INTERFACE_INTERFACE_SS_H_
#define _BE_INTERFACE_INTERFACE_SS_H_

/**
 * @class be_visitor_interface_ss
 *
 * @brief be_visitor_interface_ss
 *
 * This is a concrete visitor to generate the server skeletons for
 * interface
 */
class be_visitor_interface_ss : public be_visitor_interface
{
public:
  be_visitor_interface_ss (be_visitor_context *ctx);
  ~be_visitor_interface_ss (void);

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
  virtual void dispatch_method (be_interface *node);

  virtual int generate_amh_classes (be_interface *node);
  virtual int generate_proxy_classes (be_interface *node);
  virtual int generate_copy_ctor (be_interface *node,
                                  TAO_OutStream *os);
  virtual ACE_CString generate_flat_name (be_interface *node);
  virtual ACE_CString generate_local_name (be_interface *node);
  virtual ACE_CString generate_full_skel_name (be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SS_H_ */
