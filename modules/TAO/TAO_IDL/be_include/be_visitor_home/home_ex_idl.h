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
//    home_ex_idl.h
//
// = DESCRIPTION
//    Concrete visitor for the Home node.
//    Provides code generation for homes in the CIAO executor IDL
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_HOME_HOME_EX_IDL_H_
#define _BE_HOME_HOME_EX_IDL_H_

class be_visitor_home_ex_idl : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_home_ex_idl
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the 
  //   CIAO executor IDL for home.
  //
  //
public:
  be_visitor_home_ex_idl (be_visitor_context *ctx);

  ~be_visitor_home_ex_idl (void);

  virtual int visit_home (be_home *node);
  
  /// To regenerate attribute & operation declarations.
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);
  
  /// To regenerate the name of an anonymous type.
  /// Anonymous arrays are not allowed.
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  
private:
  void gen_nesting_open (AST_Decl *node);
  void gen_nesting_close (AST_Decl *node);
  
  void gen_implicit (void);
  void gen_explicit (void);
  void gen_derived (void);
  
  void gen_supported (void);
  
  void gen_exception_list (UTL_ExceptList *exceptions,
                           const char *prefix = "",
                           bool init_op = false);
                           
  void gen_factories (void);
  void gen_finders (void);
  void gen_init_ops (AST_Home::INIT_LIST & list);
                     
  void gen_home_executor (void);
  
  /// The CCM preproc visitor changed the scoped name and the
  /// defined_in() for the home's members to the implied
  /// *Explicit interface, which isn't a problem unless
  /// they include exceptions raised by a contained operation.
  /// factory or finder. So we restore the members just in case.
  void restore_scope (void);
                           
private:
  be_home *node_;
  TAO_OutStream &os_;
};

#endif /* _BE_HOME_HOME_EX_IDL_H_ */
