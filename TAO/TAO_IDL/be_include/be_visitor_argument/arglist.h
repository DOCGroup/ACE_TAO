
//=============================================================================
/**
 *  @file    arglist.h
 *
 *  Visitors for generation of code for Arguments as parameter lists in the
 *  operation signature
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARGUMENT_ARGLIST_H_
#define _BE_VISITOR_ARGUMENT_ARGLIST_H_

/**
 * @class be_visitor_args_arglist
 *
 * @brief Generates the code for arguments output.
 *
 * At several stages in the code generation a node of type
 * "be_operation" is visited, and the code for its arguments must
 * be generated.
 * Sometimes the argument declarations must be generated (such as
 * in the class declaration), other the arguments names (such as
 * in method invocations).
 * This class implements the declaration output, in other words,
 * it outputs both the arguments and their types.
 */
class be_visitor_args_arglist : public be_visitor_args
{
public:
  /// constructor
  be_visitor_args_arglist (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_args_arglist () override;

  /// visit the argument node
  int visit_argument (be_argument *node) override;

  // =visit all the nodes that can be the types for the argument

  /// visit array type
  int visit_array (be_array *node) override;

  /// visit the enum node
  int visit_enum (be_enum *node) override;

  /// visit interface
  int visit_interface (be_interface *node) override;

  /// visit interface forward
  int visit_interface_fwd (be_interface_fwd *node) override;

  /// visit component
  int visit_component (be_component *node) override;

  /// visit component forward
  int visit_component_fwd (be_component_fwd *node) override;

  /// visit native node
  int visit_native (be_native *node) override;

  /// visit predefined type
  int visit_predefined_type (be_predefined_type *node) override;

  /// visit sequence type
  int visit_sequence (be_sequence *node) override;

  /// visit string type
  int visit_string (be_string *node) override;

  /// visit structure type
  int visit_structure (be_structure *node) override;

  /// visit union type
  int visit_union (be_union *node) override;

  /// visit the typedef type
  int visit_typedef (be_typedef *node) override;

  /// visit valuetype
  int visit_valuetype (be_valuetype *node) override;

  /// visit valuetype_fwd
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

  /// visit eventtype
  int visit_eventtype (be_eventtype *node) override;

  /// visit eventtype_fwd
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;

  /// visit home
  int visit_home (be_home *node) override;

  /// visit valuebox
  int visit_valuebox (be_valuebox *node) override;

  /// Set the member.
  void unused (bool val);

private:
  int emit_common (be_type *node);

private:
  bool unused_;
};

#endif /* _BE_VISITOR_ARGUMENT_ARGLIST_H_ */
