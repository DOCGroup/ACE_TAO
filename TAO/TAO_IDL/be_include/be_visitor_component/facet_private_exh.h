/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_private_exh.h
 *
 *  $Id$
 *
 *  This provides code generation for private facet members
 *  in the exec impl header,
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_FACET_PRIVATE_EXH_H_
#define _BE_COMPONENT_FACET_PRIVATE_EXH_H_

class be_visitor_facet_private_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_facet_private_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation's private facet
  //   executor member(s).
  //
  //
public:
  be_visitor_facet_private_exh (be_visitor_context *ctx);

  ~be_visitor_facet_private_exh (void);

  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_COMPONENT_FACET_PRIVATE_EXH_H_ */

