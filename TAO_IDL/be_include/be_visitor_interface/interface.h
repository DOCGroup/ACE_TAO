/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This one provides the generic visitor for interfaces from which others
 *  will inherit.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_INTERFACE_INTERFACE_H_
#define _BE_INTERFACE_INTERFACE_H_

// = DESCRIPTION
//   Interface code generation is complex, the client stubs require
//   two different class and the server also require other two.
//   Only one of them includes whatever nested typedefs, exceptions
//   and or structures are defined in the interface.
//   To break this complexity a different visitor is used for each
//   case.
//
// = NOTE
//   Visitors were added recently into the compiler, so some
//   code is still generated using the old "state oriented" approach;
//   notably only the _collocated class is actually generated using
//   the visitor, in the other cases (the interface class and the
//   skeleton class) we still call back the old code generation
//   methods.
//

class AST_Operation;

/**
 * @class be_visitor_interface
 *
 * @brief be_visitor_interface
 *
 * This is a concrete visitor for interface that abstracts
 * all common tasks.
 */
class be_visitor_interface : public be_visitor_scope
{
public:
  be_visitor_interface (be_visitor_context *ctx);
  ~be_visitor_interface (void);

  virtual int visit_interface (be_interface *node);

  /// An override of the base class method so we can generate code for
  /// any abstract parents the interface may have.
  virtual int visit_scope (be_scope *node);

  virtual int visit_attribute (be_attribute *node);
  virtual int visit_constant (be_constant *node);
  virtual int visit_native (be_native *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
  virtual int visit_typedef (be_typedef *node);

 protected:
    int is_amh_rh_node (be_interface *node);

    static void add_abstract_op_args (AST_Operation *old_op,
                                      be_operation &new_op);
};

#endif /*  _BE_INTERFACE_INTERFACE_H_ */
