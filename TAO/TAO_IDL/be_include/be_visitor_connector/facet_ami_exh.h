/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_ami_exh.h
 *
 *  $Id$
 *
 *  This provides code generation for facets in the AMI connector
 *  impl header,
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_FACET_AMI_EXH_H_
#define _BE_COMPONENT_FACET_AMI_EXH_H_

class be_visitor_facet_ami_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_facet_ami_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the AMI connector implementation header for facets.
  //
  //
public:
  be_visitor_facet_ami_exh (be_visitor_context *ctx);
  
  ~be_visitor_facet_ami_exh (void);
  
  virtual int visit_provides (be_provides *node);
  virtual int visit_operation (be_operation *node);
  
private:
  int gen_reply_handler_class (void);
  int gen_facet_executor_class (void);
  
private:
  be_interface *iface_;
};

#endif /* _BE_COMPONENT_FACET_AMI_EXH_H_ */

