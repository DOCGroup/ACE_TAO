/* -*- C++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ctor.h
//
// = DESCRIPTION
//    Visitor for Exceptions.
//    For code generation for the special constructor for exceptions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_CTOR_H_
#define _BE_VISITOR_EXCEPTION_CTOR_H_

class be_visitor_exception_ctor : public be_visitor_scope
{
  // =TITLE
  //   be_visitor_exception_ctor
  //
  // =DESCRIPTION
  //   generation of the special constructor
public:
  be_visitor_exception_ctor (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception_ctor (void);
  // dtor

  virtual int post_process (be_decl *);
  // post processing after each element

  virtual int visit_exception (be_exception *node);
  // visit exception. We provide code for this method in the derived class

  virtual int visit_field (be_field *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the field

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

#endif /* _BE_VISITOR_EXCEPTION_CTOR_H_ */
