//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    home_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the Home node.
//    This provides for code generation in the servant header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_HOME_HOME_SVH_H_
#define _BE_HOME_HOME_SVH_H_

class be_visitor_home_svh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_home_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant header for component.
  //
  //
public:
  be_visitor_home_svh (be_visitor_context *ctx);
  
  ~be_visitor_home_svh (void);
  
  virtual int visit_home (be_home *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_factory (be_factory *node);
  virtual int visit_finder (be_finder *node);
  
private:
  int gen_servant_class (void);
  
  void gen_entrypoint (void);
  
private:
  be_home *node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_HOME_HOME_SVH_H_ */

