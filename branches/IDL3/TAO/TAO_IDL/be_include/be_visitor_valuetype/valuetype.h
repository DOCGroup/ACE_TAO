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

public:

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

  virtual int visit_structure_fwd (be_structure_fwd *node);
  // visit a forward declared structure

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_union_fwd (be_union_fwd *node);
  // visit a forward declared union

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef node

  virtual int visit_field (be_field *node);
  // visit a field

  // =helper methods for generation of fields

  int gen_pd (be_valuetype *node);
  // private data fields for scope

  int gen_field_pd (be_field *node);
  // private data for field

  virtual int gen_init_defn (be_valuetype *node);
  // generate the _init class definition

  virtual int gen_init_impl (be_valuetype *node);
  // generate the _init implementation

protected:

  // There are three possible situations.
  // (1) If there is no initializers but at least one operation.
  //     In this case we don't need to bother about factory.
  //
  // (2) There are no (operations or initializers) (i.e. only state
  //     members) then we need a concrete type-specific factory
  //     class whose create_for_unmarshal creates OBV_ class.
  //
  // (3) There is at least one operation and at least one initializer.
  //     In this case we need to generate abstract factory class.
  //
  // Here I reflect these situations.

  enum FactoryStyle
  {
    FS_UNKNOWN,
    FS_NO_FACTORY,
    FS_CONCRETE_FACTORY,
    FS_ABSTRACT_FACTORY
  };

  static FactoryStyle determine_factory_style (be_valuetype* node);
  // determine what kind of factory needed

  static idl_bool have_operation(be_valuetype* node);
  // recurse down the inheritance tree to determine
  // if valuetype has at least one operation/attribute.

  static idl_bool obv_need_ref_counter(be_valuetype* node);
  // check is VT needs a RefCounter mix-in in OBV_ class
  // suppose that we are deciding for this node

  static idl_bool obv_have_ref_counter(be_valuetype* node);
  // recurse down the inheritance tree to see
  // if node or one of its OBV_ base class already has RefCounter
};

#endif /*  _BE_VALUETYPE_VALUETYPE_H_ */
