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
//    be_visitor_structure.h
//
// = DESCRIPTION
//    Concrete visitor for the Structure class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_STRUCTURE_H)
#define TAO_BE_VISITOR_STRUCTURE_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

class be_visitor_structure : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_structure
  //
  // = DESCRIPTION
  //   This is the base visitor for structure
  //
  //
public:
  be_visitor_structure (be_visitor_context *ctx);
  // constructureor

  ~be_visitor_structure (void);
  // destructureor

  virtual int visit_structure (be_structure *node);
  // visit structure. We provide code for this method in the derived class

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_field (be_field *node);
  // visit field
};

class be_visitor_structure_ch : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for structure
  //
  //
public:
  be_visitor_structure_ch (be_visitor_context *ctx);
  // constructureor

  ~be_visitor_structure_ch (void);
  // destructureor

  virtual int visit_structure (be_structure *node);
  // visit structure. We provide code for this method in the derived class

};

class be_visitor_structure_ci : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for structure
  //
  //
public:
  be_visitor_structure_ci (be_visitor_context *ctx);
  // constructureor

  ~be_visitor_structure_ci (void);
  // destructureor

  virtual int visit_structure (be_structure *node);
  // visit structure. We provide code for this method in the derived class

};

class be_visitor_structure_cs : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for structure
  //
  //
public:
  be_visitor_structure_cs (be_visitor_context *ctx);
  // constructureor

  ~be_visitor_structure_cs (void);
  // destructureor

  virtual int visit_structure (be_structure *node);
  // visit structure. We provide code for this method in the derived class

};

class be_visitor_structure_any_op_ch : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for structure that generates the Any operator
  //   declarations
  //

public:
  be_visitor_structure_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_any_op_ch (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure

  virtual int visit_field (be_field *node);
  // visit field
};

class be_visitor_structure_any_op_cs : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for structure that generates the Any operator
  //   implementations
  //

public:
  be_visitor_structure_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_any_op_cs (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure

  virtual int visit_field (be_field *node);
  // visit field
};


#endif // TAO_BE_VISITOR_STRUCTURE_H
