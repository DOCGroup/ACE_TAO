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
//    be_visitor_enum.h
//
// = DESCRIPTION
//    Concrete visitor for the enum class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_ENUM_H)
#define TAO_BE_VISITOR_ENUM_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

class be_visitor_enum_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_ch
  //
  // = DESCRIPTION
  //   This is the base visitor for enum for the header file
  //
  //
public:
  be_visitor_enum_ch (be_visitor_context *ctx);
  // conenumor

  ~be_visitor_enum_ch (void);
  // deenumor

  virtual int visit_enum (be_enum *node);
  // visit enum.

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_enum_val (be_enum_val *node);
  // visit enum_val

  // = helper

  virtual int post_process (void);
  // generate a comma after each element is processed
};

class be_visitor_enum_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_cs
  //
  // = DESCRIPTION
  //   This is the visitor for enum for the impl file
  //
  //
public:
  be_visitor_enum_cs (be_visitor_context *ctx);
  // conenumor

  ~be_visitor_enum_cs (void);
  // deenumor

  virtual int visit_enum (be_enum *node);
  // visit enum and generate the typecode

};

class be_visitor_enum_any_op_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for enum that generates the Any operator
  //   declarations
  //

public:
  be_visitor_enum_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_enum_any_op_ch (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit enum
};

class be_visitor_enum_any_op_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for enum that generates the Any operator
  //   implementations
  //

public:
  be_visitor_enum_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_enum_any_op_cs (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit enum
};

#endif // TAO_BE_VISITOR_ENUM_H
