// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    typecode_defn.h
//
// = DESCRIPTION
//    Concrete visitor for the generation of TypeCodes.
//    This one generates the definitions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_
#define _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_

#include "ace/CDR_Base.h"
#include "ace/Containers_T.h"
#include "be_visitor_decl.h"
#include "be_visitor_scope.h"

const int TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE = 1024;

class AST_Structure;
class be_interface_fwd;

class be_visitor_typecode_defn : public be_visitor_scope
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

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface

  virtual int visit_component (be_component *node);
  // visit component

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_string (be_string *node);
  // visit a string

  virtual int visit_structure (be_structure *node);
  // visit a structure

  // visit a typedef
  virtual int visit_typedef (be_typedef *node);

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_valuetype (be_valuetype * node);
  // visit a valuetype

  virtual int visit_eventtype (be_eventtype * node);
  // visit a valuetype

  // = special methods

  virtual int gen_nested_namespace_begin (be_module *node);
  // generate the nested namespace begins

  virtual int gen_nested_namespace_end (be_module *node);
  // generate the nested namespace ends

  // processing for scopes

  virtual int post_process (be_decl *);
  // do any processing after every element except the last one of the scope is
  // processed

  // data structure for handling recursive and repeated typecodes

  struct QNode
  {
    be_type *node;
    ACE_CDR::Long offset;
  };

protected:

  /// Generate the TypeCode_ptr.
  /**
   * Generate actual TypeCode instance/definition, not the supporting
   * code, e.g.  "static CORBA::TypeCode_ptr const Foo_ptr = ...".
   */
  int gen_typecode_ptr (be_type * node);

  /// Generate the base type TypeCode name.
  /**
   * Generate the fully qualified base TypeCode name.  Predominantly
   * useful for TypeCodes that contain base/content TypeCodes
   * (e.g. alias, sequence, etc).
   */
  int gen_base_typecode_name (be_type * base);

  /// Generate a TypeCode forward declaration.
  int gen_forward_declared_typecode (be_type * node);

  /// Should a TypeCode be generated for the given type?
  /**
   * This method is meant for use by TypeCode visitors that may need
   * to generate a TypeCode for its members or content type.
   */
  bool is_typecode_generation_required (be_type * node);
  
  /// Generate nested namespaces for anonymous type typecodes.
  /**
   * Utility methods to generate enclosing namespaces for
   * typecodes of bounded (w)strings, sequences and arrays.
   */
  void gen_begin_NS_for_anon (void);
  void gen_end_NS_for_anon (void);

protected:

  /// Queue to keep nodes
  ACE_Unbounded_Queue <QNode*> tc_queue_;

  /// @c true if we are detecting recursion.
  bool recursion_detect_;

private:
  //
  friend class Scoped_Compute_Queue_Guard;

  ACE_CDR::Long computed_tc_size_;
  // the tc size of the node under consideration

  ACE_CDR::Long computed_encap_len_;
  // the encap length of the node under consideration

  ACE_CDR::Long computed_scope_encap_len_;
  // the encap length of the scope of the node under consideration

  // the following are used for recursive and repeated typecodes

  ACE_CDR::Long tc_offset_;
  // current computed length of the typecode

  ACE_Unbounded_Queue <QNode*> compute_queue_;
  // queue to keep nodes

  ACE_CDR::Long scope_stack_ [TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE];
  // stores scope lens during computation

  // scope related routines

  int index_;

  int push (ACE_CDR::Long);

  int pop (ACE_CDR::Long &);

protected:

  // queue related routines

  const QNode *queue_insert (ACE_Unbounded_Queue <QNode*> &,
                             be_type *node, ACE_CDR::Long offset);

  const QNode *queue_lookup (ACE_Unbounded_Queue <QNode*> &, be_type *node);

  void queue_reset (ACE_Unbounded_Queue <QNode*> &);

};

#endif /* _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_ */
