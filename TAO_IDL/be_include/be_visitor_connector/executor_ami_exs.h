/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    executor_ami_exs.h
 *
 *  $Id$
 *
 *  This provides code generation for executors in the AMI connector
 *  impl source
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_CONNECTOR_EXECUTOR_AMI_EXS_H_
#define _BE_CONNECTOR_EXECUTOR_AMI_EXS_H_

class be_visitor_executor_ami_exs
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_executor_ami_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the AMI connector implementation source for connectors.
  //
  //
public:
  be_visitor_executor_ami_exs (be_visitor_context *ctx);

  ~be_visitor_executor_ami_exs (void);

  virtual int visit_connector (be_connector *node);
  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_CONNECTOR_EXECUTOR_AMI_EXS_H_ */

