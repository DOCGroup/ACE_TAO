
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
  be_visitor_map_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_map_cdr_op_cs () = default;

  /// visit map
  virtual int visit_map (be_map *node);

  // = Visitor methods on map types

  /// visit map
  virtual int visit_array (be_array *node);

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// visit an interface forward node
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit a component
  virtual int visit_component (be_component *node);

  /// visit a component forward node
  virtual int visit_component_fwd (be_component_fwd *node);

  /// visit a home
  virtual int visit_home (be_home *node);

  /// visit valuebox
  virtual int visit_valuebox (be_valuebox *node);

  /// visit a valuetype
  virtual int visit_valuetype (be_valuetype *node);

  /// visit a valuetype forward node
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  /// visit an eventtype
  virtual int visit_eventtype (be_eventtype *node);

  /// visit an eventtype forward node
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

  /// visit a predefined type node
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit string
  virtual int visit_string (be_string *node);

  /// visit structure
  virtual int visit_structure (be_structure *node);

  /// visit typedef
  virtual int visit_typedef (be_typedef *node);

  /// visit union
  virtual int visit_union (be_union *node);

protected:
  /// helper that does the common job
  int visit_node (be_type *);
};

#endif /* _BE_VISITOR_MAP_CDR_OP_CS_H_ */
