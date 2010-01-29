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
//    factory_svs.h
//
// = DESCRIPTION
//    Concrete visitor for home factory and finder nodes.
//    This provides for code generation in the servant source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_HOME_FACTORY_SVS_H_
#define _BE_HOME_FACTORY_SVS_H_

class be_visitor_factory_svs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_factory_svs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant
  //   source for a home factory or finder.
  //
  //
public:
  be_visitor_factory_svs (be_visitor_context *ctx,
                          be_home *h_node,
                          AST_Component *comp,
                          bool for_finder);
  
  ~be_visitor_factory_svs (void);
  
  virtual int visit_factory (be_factory *node);
  
private:
  be_home *h_node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  bool for_finder_;
  bool unused_args_;
};

#endif /* _BE_HOME_FACTORY_SVS_H_ */

