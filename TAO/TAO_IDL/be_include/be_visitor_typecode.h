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
//    be_visitor_typecode.h
//
// = DESCRIPTION
//    Concrete visitor to generate code for TypeCodes
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_TYPECODE_H)
#define TAO_BE_VISITOR_TYPECODE_H

#include "be_visitor_decl.h"

class be_visitor_typecode_decl : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_typecode_decl
  //
  // = DESCRIPTION
  //    This is a concrete visitor for generating TypeCode declarations
  //

public:
  be_visitor_typecode_decl (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typecode_decl (void);
  // destructor

  virtual int visit_type (be_type *node);
  // visit a generic be_type node

  // =visit methods on all types. They call the generic visit_type

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_exception (be_exception *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef

  virtual int visit_union (be_union *node);
  // visit a union

};


class be_visitor_typecode_defn : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_typecode_defn
  //
  // = DESCRIPTION
  //    This is a concrete visitor for generating TypeCode definitions
  //

public:
  be_visitor_typecode_defn (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typecode_defn (void);
  // destructor

  virtual int visit_type (be_type *node);
  // visit a generic be_type node

  // =visit methods on all types. They call the generic visit_type

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_exception (be_exception *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef

  virtual int visit_union (be_union *node);
  // visit a union


  // = special methods

  virtual int gen_nested_namespace_begin (be_module *node);
  // generate the nested namespace begins

  virtual int gen_nested_namespace_end (be_module *node);
  // generate the nested namespace ends
};

#endif /* TAO_BE_VISITOR_TYPECODE_H */
