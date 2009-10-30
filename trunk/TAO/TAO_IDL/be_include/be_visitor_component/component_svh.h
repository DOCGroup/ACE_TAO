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
//    component_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation in the servant header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SVH_H_
#define _BE_COMPONENT_COMPONENT_SVH_H_

class be_visitor_component_svh : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant header for component.
  //
  //
public:
  be_visitor_component_svh (be_visitor_context *ctx);
  
  ~be_visitor_component_svh (void);
  
  virtual int visit_component (be_component *node);
  
private:
  void gen_entrypoint (be_component *node);
                  
private:
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_COMPONENT_COMPONENT_SVH_H_ */

