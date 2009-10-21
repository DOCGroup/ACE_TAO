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
//    component_scope.h
//
// = DESCRIPTION
//    Base class for visitors that need to visit the scope of a 
//    component and its ancestors, if any.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SCOPE_H_
#define _BE_COMPONENT_COMPONENT_SCOPE_H_

class be_visitor_component_scope : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_component_scope
  //
  // = DESCRIPTION
  //   This is an abstract visitor providing a scope visitation
  //   method that traverses the base component recursively.
  //
protected:
  be_visitor_component_scope (be_visitor_context *ctx);
  
  virtual ~be_visitor_component_scope (void);
  
public:
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_mirror_port (be_mirror_port *node);
  virtual int visit_porttype (be_porttype *node);

  int visit_component_scope (be_component *node);
  
protected:
  be_component *node_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
  bool swapping_;
  bool static_config_;
};

#endif /* _BE_COMPONENT_COMPONENT_SCOPE_H_ */

