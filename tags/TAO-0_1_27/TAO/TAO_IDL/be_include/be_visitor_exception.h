/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_exception.h
//
// = DESCRIPTION
//    Concrete visitor for the Exception class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_EXCEPTION_H)
#define TAO_BE_VISITOR_EXCEPTION_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

class be_visitor_exception : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_exception
  //
  // = DESCRIPTION
  //   This is the base visitor for exception
  //
  //
public:
  be_visitor_exception (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception (void);
  // dtor

  virtual int visit_exception (be_exception *node);
  // visit exception. We provide code for this method in the derived class

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_field (be_field *node);
  // visit field
};

class be_visitor_exception_ch : public be_visitor_exception
{
  //
  // = TITLE
  //   be_visitor_exception_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for exception
  //
  //
public:
  be_visitor_exception_ch (be_visitor_context *ctx);
  // constructureor

  ~be_visitor_exception_ch (void);
  // destructureor

  virtual int visit_exception (be_exception *node);
  // visit exception. We provide code for this method in the derived class

};

class be_visitor_exception_ci : public be_visitor_exception
{
  //
  // = TITLE
  //   be_visitor_exception_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for exception
  //
  //
public:
  be_visitor_exception_ci (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception_ci (void);
  // dtor

  virtual int visit_exception (be_exception *node);
  // visit exception. We provide code for this method in the derived class

};

class be_visitor_exception_cs : public be_visitor_exception
{
  //
  // = TITLE
  //   be_visitor_exception_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for exception
  //
  //
public:
  be_visitor_exception_cs (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception_cs (void);
  // dtor

  virtual int visit_exception (be_exception *node);
  // visit exception. We provide code for this method in the derived class

};

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

  virtual int post_process (void);
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

class be_visitor_exception_any_op_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_exception_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for exception that generates the Any operator
  //   declarations
  //

public:
  be_visitor_exception_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_exception_any_op_ch (void);
  // destructor

  virtual int visit_exception (be_exception *node);
  // visit exception

  virtual int visit_field (be_field *node);
  // visit field
};

class be_visitor_exception_any_op_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_exception_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for exception that generates the Any operator
  //   implementations
  //

public:
  be_visitor_exception_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_exception_any_op_cs (void);
  // destructor

  virtual int visit_exception (be_exception *node);
  // visit exception

  virtual int visit_field (be_field *node);
  // visit field
};

#endif // TAO_BE_VISITOR_EXCEPTION_H
