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

  ~be_visitor_amh_interface_ss (void);

protected:
  virtual void this_method (be_interface *node);
  virtual void dispatch_method (be_interface *node);

  virtual int generate_amh_classes (be_interface *node);
  virtual int generate_proxy_classes (be_interface *node);
  virtual ACE_CString generate_local_name (be_interface *node);
};

#endif /* AMH_INTERFACE_SS_H */
