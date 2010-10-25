/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_exh.h
 *
 *  $Id$
 *
 *  This provides code generation for facets in the exec impl header,
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_FACET_EXH_H_
#define _BE_COMPONENT_FACET_EXH_H_

class be_visitor_facet_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_facet_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation header for facets.
  //
  //
public:
  be_visitor_facet_exh (be_visitor_context *ctx);

  ~be_visitor_facet_exh (void);

  virtual int visit_provides (be_provides *node);
  
  static int method_helper (be_interface *derived,
                            be_interface *node,
                            TAO_OutStream *os);

private:
  const char *comment_start_border_;
  const char *comment_end_border_;
};

#endif /* _BE_COMPONENT_FACET_EXH_H_ */

