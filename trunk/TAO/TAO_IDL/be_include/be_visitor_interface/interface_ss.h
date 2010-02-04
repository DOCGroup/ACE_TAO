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
  /// constructor
  be_visitor_interface_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_ss (void);

  /// set the right context and make a visitor
  virtual int visit_interface (be_interface *node);

  /// Helper to generate the declarations for the operations
  /// of any abstract parents we may have.
  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);

protected:
  virtual void this_method (be_interface *node);
  virtual void dispatch_method (be_interface *node);

  // Some AMH skeletons must explicitly send the reply, for regular
  // classes the following function is empty, for AMH code it
  // generates the explicit call.
  virtual void generate_send_reply (TAO_OutStream * os);

///   virtual int generate_downcast_implementation (be_interface *node,
///                                                 TAO_OutStream *os);
  virtual int generate_amh_classes (be_interface *node);
  virtual int generate_proxy_classes (be_interface *node);
  virtual int generate_copy_ctor (be_interface *node,
                                  TAO_OutStream *os);
  virtual ACE_CString generate_flat_name (be_interface *node);
  virtual ACE_CString generate_local_name (be_interface *node);
  virtual ACE_CString generate_full_skel_name (be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SS_H_ */
