/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    executor_private_exh.h
 *
 *  $Id$
 *
 *  This provides code generation for private members
 *  in the exec impl header,
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_EXECUTOR_PRIVATE_EXH_H_
#define _BE_COMPONENT_EXECUTOR_PRIVATE_EXH_H_

/**
 * This is a concrete visitor to generate
 * the executor implementation's private facet
 * executor member(s).
 */
class be_visitor_executor_private_exh
  : public be_visitor_component_scope
{
public:
  be_visitor_executor_private_exh (be_visitor_context *ctx);

  ~be_visitor_executor_private_exh (void);

  virtual int visit_provides (be_provides *node);
  virtual int visit_attribute (be_attribute *node);
};

#endif /* _BE_COMPONENT_EXECUTOR_PRIVATE_EXH_H_ */

