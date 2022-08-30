/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typecode_decl.h
 *
 *  Concrete visitor for the generation of TypeCodes.
 *  This one generates the declaration.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_TYPECODE_TYPECODE_DECL_H_
#define _BE_VISITOR_TYPECODE_TYPECODE_DECL_H_

/**
 * @class be_visitor_typecode_decl
 *
 * @brief be_visitor_typecode_decl
 *
 * This is a concrete visitor for generating TypeCode declarations
 */
class be_visitor_typecode_decl : public be_visitor_decl
{

public:
  be_visitor_typecode_decl (be_visitor_context *ctx);
  ~be_visitor_typecode_decl () override;

  /// visit a generic be_type node.
  int visit_type (be_type *node) override;

  int visit_array (be_array *node) override;
  int visit_enum (be_enum *node) override;
  int visit_exception (be_exception *node) override;
  int visit_interface (be_interface *node) override;
  int visit_component (be_component *node) override;
  int visit_connector (be_connector *node) override;
  int visit_home (be_home *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_structure (be_structure *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_union (be_union *node) override;
  int visit_valuebox (be_valuebox *node) override;
  int visit_valuetype (be_valuetype *node) override;
  int visit_eventtype (be_eventtype *node) override;
};

#endif /* _BE_VISITOR_TYPECODE_TYPECODE_DECL_H_ */
