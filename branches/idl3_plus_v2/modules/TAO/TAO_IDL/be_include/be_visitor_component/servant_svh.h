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
//    servant_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation for the Servant
//    class declaration
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_SERVANT_SVH_H_
#define _BE_COMPONENT_SERVANT_SVH_H_

class be_visitor_servant_svh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_servant_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant class decl
  //   for a component.
  //
  //
public:
  be_visitor_servant_svh (be_visitor_context *ctx);
  
  ~be_visitor_servant_svh (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_consumes (be_consumes *node);
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_mirror_port (be_mirror_port *node);
  
private:
  int gen_servant_r (be_component *node);
                  
  /// Base class overrides and CIAO-specific code.                
  void gen_non_type_specific (void);
  
private:
  be_component *node_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_COMPONENT_SERVANT_SVH_H_ */

