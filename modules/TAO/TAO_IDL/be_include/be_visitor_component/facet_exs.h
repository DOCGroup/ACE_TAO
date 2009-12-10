//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    facet_exs.h
//
// = DESCRIPTION
//    This provides code generation for facets in the exec impl source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_FACET_EXS_H_
#define _BE_COMPONENT_FACET_EXS_H_

class be_visitor_facet_exs
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_facet_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation source for facets.
  //
  //
public:
  be_visitor_facet_exs (be_visitor_context *ctx);
  
  ~be_visitor_facet_exs (void);
  
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_provides (be_provides *node);
  
private:
  be_interface *op_scope_;
  const char *comment_border_;
  const char *your_code_here_;
};

#endif /* _BE_COMPONENT_FACET_EXS_H_ */

