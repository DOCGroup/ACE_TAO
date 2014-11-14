// -*- C++ -*-

//=============================================================================
/**
 *  @file    typecode_defn.h
 *
 *  $Id$
 *
 *  Concrete visitor for the generation of TypeCodes.
 *  This one generates the definitions.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_
#define _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_

#include "ace/CDR_Base.h"
#include "ace/Containers_T.h"
#include "be_visitor_decl.h"
#include "be_visitor_scope.h"

const unsigned long TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE = 1024;

class AST_Structure;
class be_interface_fwd;

/**
 * @class be_visitor_typecode_defn
 *
 * @brief be_visitor_typecode_defn
 *
 * This is a concrete visitor for generating TypeCode definitions
 */
class be_visitor_typecode_defn : public be_visitor_scope
{
public:
  be_visitor_typecode_defn (be_visitor_context *ctx);
  ~be_visitor_typecode_defn (void);

  virtual int visit_array (be_array *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);
  virtual int visit_valuetype (be_valuetype * node);
  virtual int visit_eventtype (be_eventtype * node);

  /// Processing for scopes.

  /// do any processing after every element except the last one of the scope is
  /// processed
  virtual int post_process (be_decl *);

  // Data structure for handling recursive and repeated typecodes.

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

  /// the tc size of the node under consideration
  // ACE_CDR::Long computed_tc_size_;

  /// the encap length of the node under consideration
  ACE_CDR::Long computed_encap_len_;

  /// the encap length of the scope of the node under consideration
  ACE_CDR::Long computed_scope_encap_len_;

  // the following are used for recursive and repeated typecodes

  /// current computed length of the typecode
  // ACE_CDR::Long tc_offset_;

  /// queue to keep nodes
  ACE_Unbounded_Queue <QNode*> compute_queue_;

  /// stores scope lens during computation
  ACE_CDR::Long scope_stack_ [TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE];

  // scope related routines

  unsigned long index_;

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
