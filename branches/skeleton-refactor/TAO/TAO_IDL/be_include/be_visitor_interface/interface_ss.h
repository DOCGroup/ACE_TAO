// -*- C++ -*-
//
// $Id$


// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ss.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the server skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_SS_H_
#define _BE_INTERFACE_INTERFACE_SS_H_

class be_visitor_interface_ss : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   interface
  //
public:
  be_visitor_interface_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ss (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor

  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
  // Helper to generate the declarations for the operations
  // of any abstract parents we may have.

protected:
  virtual void this_method (be_interface *node);
  virtual void dispatch_method (be_interface *node);

  // Some AMH skeletons must explicitly send the reply, for regular
  // classes the following function is empty, for AMH code it
  // generates the explicit call.
  virtual void generate_send_reply (TAO_OutStream * os);

  virtual int generate_amh_classes (be_interface *node);
  virtual int generate_proxy_classes (be_interface *node);
//   virtual int generate_downcast_implementation (be_interface *node,
//                                                 TAO_OutStream *os);
  virtual int generate_copy_ctor (be_interface *node,
                                  TAO_OutStream *os);
  virtual ACE_CString generate_flat_name (be_interface *node);
  virtual ACE_CString generate_local_name (be_interface *node);
  virtual ACE_CString generate_full_skel_name (be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SS_H_ */
