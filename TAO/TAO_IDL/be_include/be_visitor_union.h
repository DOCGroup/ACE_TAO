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
//    be_visitor_union.h
//
// = DESCRIPTION
//    Concrete visitor for the Union class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_UNION_H)
#define TAO_BE_VISITOR_UNION_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

class be_visitor_union : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_union
  //
  // = DESCRIPTION
  //   This is the base visitor for union
  //
  //
public:
  be_visitor_union (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_union_branch (be_union_branch *node);
  // visit union_branch
};

class be_visitor_union_ch : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for union
  //
  //
public:
  be_visitor_union_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_ch (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

};

class be_visitor_union_ci : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for union
  //
  //
public:
  be_visitor_union_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_ci (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

};

class be_visitor_union_cs : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for union
  //
  //
public:
  be_visitor_union_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_cs (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

};

class be_visitor_union_discriminant_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_union_discriminant_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for union
  //   discriminant
  //
  //
public:
  be_visitor_union_discriminant_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_discriminant_ch (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef node
};

class be_visitor_union_discriminant_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_union_discriminant_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for union
  //   discriminant
  //
  //
public:
  be_visitor_union_discriminant_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_discriminant_ci (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef node
};

class be_visitor_union_discriminant_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_union_discriminant_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for union
  //   discriminant
  //
  //
public:
  be_visitor_union_discriminant_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_discriminant_cs (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit an enum. Required to generate the typecode for an enum definition
  // appearing side the union

};

class be_visitor_union_any_op_ch : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the Any operator
  //   declarations
  //

public:
  be_visitor_union_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_any_op_ch (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union

  virtual int visit_union_branch (be_union_branch *node);
  // visit union_branch
};

class be_visitor_union_any_op_cs : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the Any operator
  //   implementations
  //

public:
  be_visitor_union_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_any_op_cs (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union

  virtual int visit_union_branch (be_union_branch *node);
  // visit union_branch
};

#endif // TAO_BE_VISITOR_UNION_H
