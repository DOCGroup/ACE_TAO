//=============================================================================
/**
 *  @file   amh_rh_ss.h
 *
 *  $Id$
 *
 *  Specialized visitor for AMH-ResponseHandler that generates code for the
 *  RH in the skeleton source file
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 */
//=============================================================================

#ifndef AMH_RH_INTERFACE_SS_H
#define AMH_RH_INTERFACE_SS_H


class be_visitor_amh_rh_interface_ss : public be_visitor_interface_ss
{
public:
  be_visitor_amh_rh_interface_ss (be_visitor_context *ctx);

  int visit_interface (be_interface *node);

  int visit_operation (be_operation *node);

  ~be_visitor_amh_rh_interface_ss (void);
};

#endif /* AMH_RH_INTERFACE_SS_H */
