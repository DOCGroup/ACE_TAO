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
//    valuetype.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuetype node.
//    This one provides the generic visitor for valuetypes from which others
//    will inherit.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on interface.h from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_H_
#define _BE_VALUETYPE_VALUETYPE_H_


class be_visitor_valuetype : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_valuetype
  //
  // = DESCRIPTION
  //   This is a concrete visitor for valuetype that abstracts all common tasks
  //

public:
  be_visitor_valuetype (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // visit the valuetype node

  virtual int visit_valuetype_scope (be_valuetype *node);
  // visit the scope of the valuetype node
  // (in public/private field order)

  virtual void begin_public ();
  virtual void begin_private ();

 // =visit methods on all elements syntactically valid in a Valuetype scope

  virtual int visit_attribute (be_attribute *node);
  // visit attribute

  virtual int visit_constant (be_constant *node);
  // visit a constant

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_operation (be_operation *node);
  // visit operation

  virtual int visit_exception (be_exception *node);
  // visit exception (not used)

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef node

  virtual int visit_field (be_field *node);
  // visit a field

  // =helper methods for generation of fields

  //int gen_field_i (be_field *node, idl_bool make_inline);
  // field accessor and modifier implemantation

  int gen_pd (be_valuetype *node);
  // private data fields for scope

  int gen_field_pd (be_field *node);
  // private data for field

  virtual int gen_init_defn (be_valuetype *node);
  // generate the _init class definition

  virtual int gen_init_impl (be_valuetype *node);
  // generate the _init implementation

};

#endif /*  _BE_VALUETYPE_VALUETYPE_H_ */
