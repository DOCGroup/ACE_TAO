//=============================================================================
/**
 *  @file   amh_rh_sh.h
 *
 *  $Id$
 *
 *  Specialized visitor for AMH ResponseHandler (RH) that generates
 *  code for the RH interface in the skeleton header
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 */
//=============================================================================

#ifndef AMH_RH_SH_H_
#define AMH_RH_SH_H_

class be_visitor_amh_rh_interface_sh : public be_visitor_interface_sh
{
public:
  be_visitor_amh_rh_interface_sh (void);

  be_visitor_amh_rh_interface_sh (be_visitor_context *ctx);

  ~be_visitor_amh_rh_interface_sh (void);

  virtual int visit_interface (be_interface *node);

  virtual int visit_operation (be_operation *node);
};

#endif /* AMH_RH_SH_H_ */
