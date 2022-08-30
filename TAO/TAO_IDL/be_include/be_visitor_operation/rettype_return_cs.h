
//=============================================================================
/**
 *  @file    rettype_return_cs.h
 *
 *  Visitor for generating code for IDL operations. This generates the return
 *  statement for returning the return type variable in the client stub.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_RETTYPE_RETURN_CS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_RETURN_CS_H_

// ************************************************************
// Operation visitor for returning the return val
// ************************************************************

/**
 * @class be_visitor_operation_rettype_return_cs
 *
 * @brief be_visitor_operation_rettype_return_cs
 *
 * This is a visitor to generate a variable declaration ofr an operation
 * return type
 */
class be_visitor_operation_rettype_return_cs :public be_visitor_decl
{
public:
  /// constructor
  be_visitor_operation_rettype_return_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_rettype_return_cs ();

  /// visit an array node
  int visit_array (be_array *node) override;

  /// visit an enum node
  int visit_enum (be_enum *node) override;

  /// visit an interface node
  int visit_interface (be_interface *node) override;

  /// visit an interface node
  int visit_interface_fwd (be_interface_fwd *node) override;

  /// visit a valuebox node
  int visit_valuebox (be_valuebox *node) override;

  /// visit valuetype
  int visit_valuetype (be_valuetype *node) override;

  /// visit valuetype forward
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

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

  /// visit component
  int visit_component (be_component *node) override;

  /// visit component forward
  int visit_component_fwd (be_component_fwd *node) override;

  /// visit eventtype
  int visit_eventtype (be_eventtype *node) override;

  /// visit eventtype forward
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;

  /// visit component home
  int visit_home (be_home *node) override;
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_RETURN_CS_H_ */
