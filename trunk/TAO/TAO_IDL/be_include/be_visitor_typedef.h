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
//    be_visitor_typedef.h
//
// = DESCRIPTION
//    Concrete visitor for the Typedef class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_TYPEDEF_H)
#define TAO_BE_VISITOR_TYPEDEF_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

class be_visitor_typedef : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_typedef
  //
  // = DESCRIPTION
  //    This is a concrete visitor for typedef that abstracts all common tasks
  //

public:
  be_visitor_typedef (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // must be overridden

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type node

  virtual int visit_string (be_string *node);
  // visit a typedef

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

};

// we need derived typedef visitors for the client files.

class be_visitor_typedef_ch : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for typedef
  //
  //
public:
  be_visitor_typedef_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_ch (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef. We provide code for this method in the derived class

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit an interface;

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type node

  virtual int visit_string (be_string *node);
  // visit a typedef

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

};

class be_visitor_typedef_ci : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for typedef
  //
  //
public:
  be_visitor_typedef_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_ci (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef. We provide code for this method in the derived class

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

};

class be_visitor_typedef_cs : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for typedef
  //
  //
public:
  be_visitor_typedef_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_cs (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef. We provide code for this method in the derived class

};

class be_visitor_typedef_any_op_ch : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for typedef that generates the Any operator
  //   declarations and implementations
  //

public:
  be_visitor_typedef_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_any_op_ch (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

};

class be_visitor_typedef_any_op_cs : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for typedef that generates the Any operator
  //   declarations and implementations
  //

public:
  be_visitor_typedef_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_any_op_cs (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

};


#endif // TAO_BE_VISITOR_TYPEDEF_H
