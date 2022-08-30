
//=============================================================================
/**
 *  @file    marshal_ss.h
 *
 *  Visitors for generation of code for Arguments. This generates the
 *  code to pass the arguments to the call to the marshaler/demarshaler
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARGUMENT_MARSHAL_SS_H_
#define _BE_VISITOR_ARGUMENT_MARSHAL_SS_H_

/**
 * @class be_visitor_args_marshal_ss
 *
 * @brief be_visitor_args_marshal_ss
 *
 * Code to be generated when making the invocation
 */
class be_visitor_args_marshal_ss : public be_visitor_args
{
public:
  /// constructor
  be_visitor_args_marshal_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_args_marshal_ss () override;

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

  /// visit valuebox
  int visit_valuebox (be_valuebox *node) override;

  /// visit valuetype
  int visit_valuetype (be_valuetype *node) override;

  /// visit valuetype forward
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

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

  /// visit a component node
  int visit_component (be_component *node) override;

  /// visit a forward declared component node
  int visit_component_fwd (be_component_fwd *node) override;

  /// visit an eventtype node
  int visit_eventtype (be_eventtype *node) override;

  /// visit a forward declared eventtype node
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;

  /// visit home
  int visit_home (be_home *node) override;

 private:
  int emit_common ();
  int emit_common2 (be_type *node);
};

#endif  /* _BE_VISITOR_ARGUMENT_MARSHAL_SS_H_ */
