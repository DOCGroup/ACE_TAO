/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_exs.h
 *
 *  $Id$
 *
 *  This provides code generation for facets in the exec impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_FACET_EXS_H_
#define _BE_COMPONENT_FACET_EXS_H_

/**
 * This is a concrete visitor to generate
 * the executor implementation source for facets.
 */
class be_visitor_facet_exs
  : public be_visitor_component_scope
{
public:
  be_visitor_facet_exs (be_visitor_context *ctx);

  ~be_visitor_facet_exs (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_provides (be_provides *node);

private:
  be_decl *op_scope_;
  const char *comment_start_border_;
  const char *comment_end_border_;
};

#endif /* _BE_COMPONENT_FACET_EXS_H_ */

