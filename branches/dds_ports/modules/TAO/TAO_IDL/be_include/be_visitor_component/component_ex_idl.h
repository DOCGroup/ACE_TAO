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
//    component_ex_idl.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    Provides code generation for components in the CIAO executor IDL
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_EX_IDL_H_
#define _BE_COMPONENT_COMPONENT_EX_IDL_H_

class be_visitor_component_ex_idl : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_component_ex_idl
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the 
  //   CIAO executor IDL for component.
  //
  //
public:
  be_visitor_component_ex_idl (be_visitor_context *ctx);

  ~be_visitor_component_ex_idl (void);

  virtual int visit_component (be_component *node);
  
  /// To regenerate attribute declarations.
  virtual int visit_attribute (be_attribute *node);
  
  /// To regenerate the name of the attribute's referenced
  /// (anonymous) type. Anonymous arrays are not allowed.
  virtual int visit_string (be_string *node);
  virtual int visit_sequence (be_sequence *node);
  
private:
  void gen_facets (void);
  
  void gen_component (void);
  void gen_executor_base (void);
  void gen_supported (void);
  void gen_executor_contents (void);
  void gen_facet_ops (void);
  void gen_consumer_ops (void);
  
  void gen_exception_list (UTL_ExceptList *exceptions,
                           const char *prefix = "",
                           bool closed = true);
                           
  void gen_context (void);
  void gen_publisher_ops (void);
  void gen_emitter_ops (void);
  void gen_receptacle_ops (void);
  
  void gen_executor_derived (void);
  
private:
  be_component *node_;
  TAO_OutStream &os_;
};

#endif /* _BE_COMPONENT_COMPONENT_EX_IDL_H_ */
