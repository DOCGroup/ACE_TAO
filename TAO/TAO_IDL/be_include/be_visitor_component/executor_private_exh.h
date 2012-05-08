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
  virtual void set_flags (bool do_provides, bool do_attribute);
private:
  bool do_provides_;
  bool do_attribute_;
};

#endif /* _BE_COMPONENT_EXECUTOR_PRIVATE_EXH_H_ */

