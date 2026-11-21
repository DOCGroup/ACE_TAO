/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface.h
 *
 *  Concrete visitor for the Interface node.
 *  This one provides the generic visitor for interfaces from which others
 *  will inherit.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_INTERFACE_INTERFACE_H_
#define _BE_INTERFACE_INTERFACE_H_

/// @note Interface code generation uses different visitors for client stub
/// and server classes. Only one of these classes includes nested typedefs,
/// exceptions, and structures declared in the interface.
///
/// Some code is still generated using the older state-oriented approach.
/// In particular, the interface and skeleton classes use the older code
/// generation methods, while the collocated class uses a visitor.
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
  explicit be_visitor_interface (be_visitor_context *ctx);
  ~be_visitor_interface () override;
  be_visitor_interface (be_visitor_interface const &) = delete;
  be_visitor_interface (be_visitor_interface &&) = delete;
  be_visitor_interface &operator= (be_visitor_interface const &) = delete;
  be_visitor_interface &operator= (be_visitor_interface &&) = delete;

  int visit_interface (be_interface *node) override;

  /// An override of the base class method so we can generate code for
  /// any abstract parents the interface may have.
  int visit_scope (be_scope *node) override;

  int visit_attribute (be_attribute *node) override;
  int visit_constant (be_constant *node) override;
  int visit_native (be_native *node) override;
  int visit_enum (be_enum *node) override;
  int visit_exception (be_exception *node) override;
  int visit_operation (be_operation *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
  int visit_typedef (be_typedef *node) override;

 protected:
    bool is_amh_rh_node (be_interface *node);

};

#endif /*  _BE_INTERFACE_INTERFACE_H_ */
