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
//    be_visitor_union_branch.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Union_Branch" node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_UNION_BRANCH_H)
#define TAO_BE_VISITOR_UNION_BRANCH_H

#include "be_visitor_decl.h"

class be_visitor_union_branch_public_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_union_branch node for the client
  //   header. This generates the code for the public section of the "union"
  //   class
  //
public:
  be_visitor_union_branch_public_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_ch (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_interface (be_interface *node);
  // visit interface type

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward type

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

class be_visitor_union_branch_private_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_private_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_union_branch node for the client
  //   header. This generates the code for the private section of the "union"
  //   class
  //
public:
  be_visitor_union_branch_private_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_private_ch (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_interface (be_interface *node);
  // visit interface type

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward type

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

class be_visitor_union_branch_public_ci : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_ci
  //
  // = DESCRIPTION
  //    This visitor is used to generate the accessors for the members of the
  //    union
  //
public:
  be_visitor_union_branch_public_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_ci (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_interface (be_interface *node);
  // visit interface type

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward type

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

class be_visitor_union_branch_public_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_cs
  //
  // = DESCRIPTION
  //    This visitor is used to generate implementation such as typecodes for
  //    constructed types
  //
public:
  be_visitor_union_branch_public_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_cs (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

};

class be_visitor_union_branch_public_assign_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_assign_cs
  //
  // = DESCRIPTION
  //    This is used to generate the body of the assignment operator and the
  //    copy constructor of the union class
  //
public:
  be_visitor_union_branch_public_assign_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_assign_cs (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

};

#endif // TAO_BE_VISITOR_UNION_BRANCH_H
