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
//    be_visitor_field.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Field" node
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_FIELD_H)
#define TAO_BE_VISITOR_FIELD_H

#include "be_visitor_decl.h"

class be_visitor_field_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client header.
  //
public:
  be_visitor_field_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_ch (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be

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

class be_visitor_field_ci : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client inline
  //   file
  //
public:
  be_visitor_field_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_ci (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be and for
  // which inline code must be generated

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

class be_visitor_field_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client stubs
  //   file
  //
public:
  be_visitor_field_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_cs (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be and impl
  // code must be generated

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

#endif // TAO_BE_VISITOR_FIELD_H
