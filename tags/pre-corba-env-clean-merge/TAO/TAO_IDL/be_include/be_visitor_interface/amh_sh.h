//=============================================================================
/**
 *  @file   amh_sh.h
 *
 *  $Id$
 *
 *  Specialized interface visitor for AMH.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef AMH_SH_H_
#define AMH_SH_H_

class be_visitor_amh_interface_sh : public be_visitor_interface_sh
{
public:
  be_visitor_amh_interface_sh (be_visitor_context *ctx);

  ~be_visitor_amh_interface_sh (void);

protected:
  virtual void this_method (be_interface *node);
};

#endif /* AMH_SH_H_ */
