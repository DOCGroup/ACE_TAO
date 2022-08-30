/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_ch.h
 *
 *  Concrete visitor for the base "BE_Field" node
 *  This generates code for structure members in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_FIELD_CH_H_
#define _BE_VISITOR_FIELD_CH_H_

/**
 * @class be_visitor_field_ch
 *
 * @brief be_visitor_field_ch
 *
 * This is a concrete visitor for the be_field node for the client header.
 */
class be_visitor_field_ch : public be_visitor_decl
{
public:
  be_visitor_field_ch (be_visitor_context *ctx);
  ~be_visitor_field_ch () override;

  int visit_field (be_field *node) override;
  int visit_array (be_array *node) override;
  int visit_enum (be_enum *node) override;
  int visit_interface (be_interface *node) override;
  int visit_interface_fwd (be_interface_fwd *node) override;
  int visit_valuebox (be_valuebox *node) override;
  int visit_valuetype (be_valuetype *node) override;
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;
  int visit_predefined_type (be_predefined_type *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_string (be_string *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
  int visit_component (be_component *node) override;
  int visit_component_fwd (be_component_fwd *node) override;
  int visit_eventtype (be_eventtype *node) override;
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;

 private:
  int emit_common (be_type *node);
};

#endif /*  _BE_VISITOR_FIELD_CH_H_ */
