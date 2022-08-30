
//=============================================================================
/**
 *  @file    rettype.h
 *
 *  Visitor for generating code for IDL operations. This generates the return
 *  type of the operation
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_RETTYPE_H_
#define _BE_VISITOR_OPERATION_RETTYPE_H_

// ***********************************************************************
// Operation visitor for return types in the signature of the operation
// ***********************************************************************

/**
 * @class be_visitor_operation_rettype
 *
 * @brief be_visitor_operation_ch
 *
 * This is a visitor to generate operation return types
 */
class be_visitor_operation_rettype : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_operation_rettype (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_rettype ();

  /// visit an array node
  int visit_array (be_array *node) override;

  /// visit an enum node
  int visit_enum (be_enum *node) override;

  /// visit an interface node
  int visit_interface (be_interface *node) override;

  /// visit an interface node
  int visit_interface_fwd (be_interface_fwd *node) override;

  /// visit native type
  int visit_native (be_native *node) override;

  /// visit a predefined type node
  int visit_predefined_type (be_predefined_type *node) override;

  /// visit a sequence node
  int visit_sequence (be_sequence *node) override;

  /// visit a sequence node
  int visit_string (be_string *node) override;

  /// visit a structure node
  int visit_structure (be_structure *node) override;

  /// visit a typedef node
  int visit_typedef (be_typedef *node) override;

  /// visit a union node
  int visit_union (be_union *node) override;

  /// visit a valuetype node
  int visit_valuetype (be_valuetype *node) override;

  /// visit a forward declared valuetype node
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

  /// visit a connector node
  int visit_connector (be_connector *node) override;

  /// visit a component node
  int visit_component (be_component *node) override;

  /// visit a forward declared component node
  int visit_component_fwd (be_component_fwd *node) override;

  /// visit an eventtype node
  int visit_eventtype (be_eventtype *node) override;

  /// visit a forward declared eventtype node
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;

  /// visit a component home node
  int visit_home (be_home *node) override;

  /// visit a valuebox node
  int visit_valuebox (be_valuebox *node) override;

private:
  virtual const char *type_name (be_type *node);

private:
  TAO_OutStream *os;
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_H_ */
