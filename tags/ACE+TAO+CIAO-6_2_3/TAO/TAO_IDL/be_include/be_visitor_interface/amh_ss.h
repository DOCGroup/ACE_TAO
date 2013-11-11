// -*- C++ -*-

//=============================================================================
/**
 *  @file   amh_ss.h
 *
 *  $Id$
 *
 *  Specialized interface visitor for AMH
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef AMH_INTERFACE_SS_H
#define AMH_INTERFACE_SS_H


class be_visitor_amh_interface_ss : public be_visitor_interface_ss
{
public:
  be_visitor_amh_interface_ss (be_visitor_context *ctx);

  int visit_interface (be_interface *node);
  int visit_operation (be_operation *node);
  int visit_attribute (be_attribute *node);

  ~be_visitor_amh_interface_ss (void);

protected:
  virtual void this_method (be_interface *node);
  virtual void dispatch_method (be_interface *node);

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

#endif /* AMH_INTERFACE_SS_H */
