
//=============================================================================
/**
 *  @file    arglist.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Arguments as parameter lists in the
 *  operation signature
 *
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
  virtual ~be_visitor_args_arglist (void);

  /// visit the argument node
  virtual int visit_argument (be_argument *node);

  // =visit all the nodes that can be the types for the argument

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit the enum node
  virtual int visit_enum (be_enum *node);

  /// visit interface
  virtual int visit_interface (be_interface *node);

  /// visit interface forward
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit component
  virtual int visit_component (be_component *node);

  /// visit component forward
  virtual int visit_component_fwd (be_component_fwd *node);

  /// visit native node
  virtual int visit_native (be_native *node);

  /// visit predefined type
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit string type
  virtual int visit_string (be_string *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit union type
  virtual int visit_union (be_union *node);

  /// visit the typedef type
  virtual int visit_typedef (be_typedef *node);

  /// visit valuetype
  virtual int visit_valuetype (be_valuetype *node);

  /// visit valuetype_fwd
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  /// visit eventtype
  virtual int visit_eventtype (be_eventtype *node);

  /// visit eventtype_fwd
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

  /// visit home
  virtual int visit_home (be_home *node);

  /// visit valuebox
  virtual int visit_valuebox (be_valuebox *node);

  /// Set the member.
  void unused (bool val);

private:
  int emit_common (be_type *node);

private:
  bool unused_;
};

#endif /* _BE_VISITOR_ARGUMENT_ARGLIST_H_ */
