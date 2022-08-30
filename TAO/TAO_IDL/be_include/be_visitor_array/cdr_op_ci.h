
//=============================================================================
/**
 *  @file    cdr_op_ci.h
 *
 *  Visitors for generation of code for Arrays in client stubs. This one
 *  generates the CDR operators.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_CDR_OP_CI_H_
#define _BE_VISITOR_ARRAY_CDR_OP_CI_H_

/**
 * @class be_visitor_array_cdr_op_ci
 *
 * @brief be_visitor_array_cdr_op_ci
 *
 * This is a concrete visitor for array that generates the CDR operator
 * implementations
 */
class be_visitor_array_cdr_op_ci : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_array_cdr_op_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array_cdr_op_ci (void);

  /// visit array
  int visit_array (be_array *node) override;

  // = Visitor methods on array types

  /// visit an enum
  int visit_enum (be_enum *node) override;

  /// visit an interface
  int visit_interface (be_interface *node) override;

  /// visit an interface forward node
  int visit_interface_fwd (be_interface_fwd *node) override;

  /// visit a valuetype
  int visit_valuetype (be_valuetype *node) override;

  /// visit a valuetype forward node
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

  /// visit a predefined type node
  int visit_predefined_type (be_predefined_type *node) override;

  /// visit sequence
  int visit_sequence (be_sequence *node) override;

  /// visit string
  int visit_string (be_string *node) override;

  /// visit structure
  int visit_structure (be_structure *node) override;

  /// visit typedef
  int visit_typedef (be_typedef *node) override;

  /// visit union
  int visit_union (be_union *node) override;

protected:
  /// helper that does the common job
  int visit_node (be_type *);
};

#endif /* _BE_VISITOR_ARRAY_CDR_OP_CI_H_ */
