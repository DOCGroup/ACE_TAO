
/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Typedef class
 *  This provides the code generation for the Any operators of the Typedef
 *  class in the client stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_TYPEDEF_ANY_OP_CS_H_
#define _BE_VISITOR_TYPEDEF_ANY_OP_CS_H_

/**
 * @class be_visitor_typedef_any_op_cs
 *
 * @brief be_visitor_typedef_any_op_cs
 *
 * This is a concrete visitor for typedef that generates the Any operator
 * declarations and implementations
 */
class be_visitor_typedef_any_op_cs : public be_visitor_typedef
{

public:
  /// constructor
  be_visitor_typedef_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_typedef_any_op_cs (void);

  /// visit typedef
  virtual int visit_typedef (be_typedef *node);

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  virtual int visit_array (be_array *node);

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit a sequence
  virtual int visit_sequence (be_sequence *node);

  /// visit a structure
  virtual int visit_structure (be_structure *node);

  /// visit a union
  virtual int visit_union (be_union *node);

};

#endif /* _BE_VISITOR_TYPEDEF_ANY_OP_CS_H_*/
