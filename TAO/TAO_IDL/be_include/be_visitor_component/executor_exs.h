/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    executor_exs.h
 *
 *  $Id$
 *
 *  This provides code generation in the exec impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_EXECUTOR_EXS_H_
#define _BE_COMPONENT_EXECUTOR_EXS_H_

class be_visitor_executor_exs
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_executor_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation source.
  //
  //
public:
  be_visitor_executor_exs (be_visitor_context *ctx);

  ~be_visitor_executor_exs (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_component (be_component *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_consumes (be_consumes *node);

private:
  be_interface *op_scope_;
  const char *comment_border_;
  const char *your_code_here_;
};

#endif /* _BE_COMPONENT_EXECUTOR_EXS_H_ */

