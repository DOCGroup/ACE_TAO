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
//    ctor_assign.h
//
// = DESCRIPTION
//    Visitor for Exceptions.
//    For code generation for the special constructor and assignment operator
//    for exceptions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_CTOR_ASSIGN_H_
#define _BE_VISITOR_EXCEPTION_CTOR_ASSIGN_H_

class be_visitor_exception_ctor_assign : public be_visitor_scope
{
  // =TITLE
  //   be_visitor_exception_ctor_assign
  //
  // =DESCRIPTION
  //   generation of the assignment statements in
public:
  be_visitor_exception_ctor_assign (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception_ctor_assign (void);
  // dtor

  virtual int visit_exception (be_exception *node);
  // visit exception.

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit all the nodes that can be the types for the field

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype forward

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

#endif /* _BE_VISITOR_EXCEPTION_CTOR_ASSIGN_H_ */
