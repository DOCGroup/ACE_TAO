
//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  Visitors for generation of code for Map in client stubs. This one
 *  generates the CDR operators.
 *
 *  @author Tyler Mayoff
 */
//=============================================================================


#ifndef _BE_VISITOR_MAP_CDR_OP_CS_H_
#define _BE_VISITOR_MAP_CDR_OP_CS_H_

/**
 * @class be_visitor_map_cdr_op_cs
 *
 * @brief be_visitor_map_cdr_op_cs
 *
 * This is a concrete visitor for map that generates the CDR operator
 * implementations
 */
class be_visitor_map_cdr_op_cs : public be_visitor_decl
{
public:
  /// constructor
  explicit be_visitor_map_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_map_cdr_op_cs () override = default;

  be_visitor_map_cdr_op_cs (const be_visitor_map_cdr_op_cs &) = delete;
  be_visitor_map_cdr_op_cs (be_visitor_map_cdr_op_cs &&) = delete;
  be_visitor_map_cdr_op_cs &operator= (const be_visitor_map_cdr_op_cs &) = delete;
  be_visitor_map_cdr_op_cs &operator= (be_visitor_map_cdr_op_cs &&) = delete;

  /// visit map
  int visit_map (be_map *node) override;

  // = Visitor methods on map types

  /// visit map
  int visit_array (be_array *node) override;

  /// visit an enum
  int visit_enum (be_enum *node) override;

  /// visit an interface
  int visit_interface (be_interface *node) override;

  /// visit an interface forward node
  int visit_interface_fwd (be_interface_fwd *node) override;

  /// visit a component
  int visit_component (be_component *node) override;

  /// visit a component forward node
  int visit_component_fwd (be_component_fwd *node) override;

  /// visit a home
  int visit_home (be_home *node) override;

  /// visit valuebox
  int visit_valuebox (be_valuebox *node) override;

  /// visit a valuetype
  int visit_valuetype (be_valuetype *node) override;

  /// visit a valuetype forward node
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

  /// visit an eventtype
  int visit_eventtype (be_eventtype *node) override;

  /// visit an eventtype forward node
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;

  /// visit a predefined type node
  int visit_predefined_type (be_predefined_type *node) override;

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

#endif /* _BE_VISITOR_MAP_CDR_OP_CS_H_ */
