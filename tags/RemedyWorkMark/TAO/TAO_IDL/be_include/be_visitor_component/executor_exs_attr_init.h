/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    executor_exs_attr_init.h
 *
 *  $Id$
 *
 *  This provides code generation in the exec impl source for
 *  the attribute members' initialization in the constructor.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_EXECUTOR_EXS_ATTR_INIT_H_
#define _BE_COMPONENT_EXECUTOR_EXS_ATTR_INIT_H_

class be_visitor_executor_exs_attr_init
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_executor_exs_attr_init
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor attribute members' initialization
  //   in the constructor.
  //
  //
public:
  be_visitor_executor_exs_attr_init (be_visitor_context *ctx);
  ~be_visitor_executor_exs_attr_init (void);

  virtual int visit_attribute (be_attribute *node);

  virtual int pre_process (be_decl *);

  bool attr_generated (void) const;

private:
  bool semicolon_generated_;
};

#endif /* _BE_COMPONENT_EXECUTOR_EXS_ATTR_INIT_H_ */

