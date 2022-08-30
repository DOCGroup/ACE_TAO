// -*- C++ -*-

//=============================================================================
/**
 *  @file   amh_ss.h
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

  int visit_interface (be_interface *node) override;
  int visit_operation (be_operation *node) override;
  int visit_attribute (be_attribute *node) override;

  ~be_visitor_amh_interface_ss ();

protected:
  void this_method (be_interface *node) override;
  void dispatch_method (be_interface *node) override;

  int generate_amh_classes (be_interface *node) override;
  int generate_proxy_classes (be_interface *node) override;
//   virtual int generate_downcast_implementation (be_interface *node,
//                                                 TAO_OutStream *os);
  int generate_copy_ctor (be_interface *node,
                          TAO_OutStream *os) override;
  ACE_CString generate_flat_name (be_interface *node) override;
  ACE_CString generate_local_name (be_interface *node) override;
  ACE_CString generate_full_skel_name (be_interface *node) override;
};

#endif /* AMH_INTERFACE_SS_H */
