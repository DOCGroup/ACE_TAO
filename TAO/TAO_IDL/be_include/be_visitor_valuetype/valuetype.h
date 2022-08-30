/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype.h
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides the generic visitor for valuetypes from which others
 *  will inherit.
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on interface.h from Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_H_
#define _BE_VALUETYPE_VALUETYPE_H_


/**
 * @class be_visitor_valuetype
 *
 * @brief be_visitor_valuetype
 *
 * This is a concrete visitor for valuetype that abstracts all common tasks
 */
class be_visitor_valuetype : public be_visitor_scope
{

public:

  /// constructor
  be_visitor_valuetype (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype () override;

public:
  /// visit the scope of the valuetype node
  /// (in public/private field order)
  virtual int visit_valuetype_scope (be_valuetype *node);

  virtual void begin_public ();
  virtual void begin_private ();

 // =visit methods on all elements syntactically valid in a Valuetype scope

  /// visit attribute
  int visit_attribute (be_attribute *node) override;

  /// visit a constant
  int visit_constant (be_constant *node) override;

  /// visit an enum
  int visit_enum (be_enum *node) override;

  /// visit exception (not used)
  int visit_exception (be_exception *node) override;

  /// visit a structure
  int visit_structure (be_structure *node) override;

  /// visit a forward declared structure
  int visit_structure_fwd (be_structure_fwd *node) override;

  /// visit a union
  int visit_union (be_union *node) override;

  /// visit a forward declared union
  int visit_union_fwd (be_union_fwd *node) override;

  /// visit the typedef node
  int visit_typedef (be_typedef *node) override;

  /// visit a field
  int visit_field (be_field *node) override;

  // =helper methods for generation of fields

  /// private data fields for scope
  int gen_pd (be_valuetype *node);

  /// private data for field
  int gen_field_pd (be_field *node);

  /// generate the _init class definition
  virtual int gen_init_defn (be_valuetype *node);

  /// generate the _init implementation
  virtual int gen_init_impl (be_valuetype *node);

  /// Recursively generate args for the initializing constuctor.
  void gen_obv_init_constructor_args (be_valuetype *node,
                                      unsigned long &index);

protected:
  /// check is VT needs a RefCounter mix-in in OBV_ class
  /// suppose that we are deciding for this node
  static bool obv_need_ref_counter (be_valuetype *node);

  /// recurse down the inheritance tree to see
  /// if node or one of its OBV_ base class already has RefCounter
  static bool obv_have_ref_counter (be_valuetype *node);

  /// Predicate common to _ch and _cs visitors.
  bool is_amh_exception_holder (be_valuetype *node);
};

#endif /*  _BE_VALUETYPE_VALUETYPE_H_ */
