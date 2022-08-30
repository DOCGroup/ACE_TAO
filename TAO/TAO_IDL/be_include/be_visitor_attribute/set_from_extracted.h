/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    set_from_extracted.h
 *
 *  Encapsulation the variations in setting read/write
 *  attributes from values extracted from an Any
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef BE_VISITOR_ATTRIBUTE_SET_FROM_EXTRACTED_H
#define BE_VISITOR_ATTRIBUTE_SET_FROM_EXTRACTED_H

/**
 * Contains overloaded methods that emit arg values to be
 * passed to an attribute mutator
 */
class be_visitor_attribute_set_from_extracted
  : public be_visitor_any_extracted_type_decl
{
public:
  be_visitor_attribute_set_from_extracted (be_visitor_context *ctx);
  ~be_visitor_attribute_set_from_extracted () override;

  int visit_array (be_array *) override;
  int visit_component (be_component *) override;
  int visit_enum (be_enum *) override;
  int visit_eventtype (be_eventtype *) override;
  int visit_home (be_home *) override;
  int visit_interface (be_interface *) override;
  int visit_predefined_type (be_predefined_type *) override;
  int visit_sequence (be_sequence *) override;
  int visit_string (be_string *) override;
  int visit_structure (be_structure *) override;
  int visit_typedef (be_typedef *) override;
  int visit_union (be_union *) override;
  int visit_valuebox (be_valuebox *) override;
  int visit_valuetype (be_valuetype *) override;
};

#endif /* BE_VISITOR_ATTRIBUTE_SET_FROM_EXTRACTED_H */

