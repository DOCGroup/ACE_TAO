/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ccm_init.h
 *
 *  Visitor generating a block of code to initialize
 *  each of a component or home's read/write attributes
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef BE_VISITOR_ATTRIBUTE_CCM_INIT_H
#define BE_VISITOR_ATTRIBUTE_CCM_INIT_H

/**
 * Generates the body of a CIAO component or home's set_attributes() method.
 */
class be_visitor_attribute_ccm_init
  : public be_visitor_any_extracted_type_decl
{
public:
  be_visitor_attribute_ccm_init (be_visitor_context *ctx);
  ~be_visitor_attribute_ccm_init ();

  int visit_attribute (be_attribute *node) override;

  int visit_array (be_array *node) override;
  int visit_component (be_component *node) override;
  int visit_enum (be_enum *node) override;
  int visit_eventtype (be_eventtype *node) override;
  int visit_home (be_home *node) override;
  int visit_interface (be_interface *node) override;
  int visit_predefined_type (be_predefined_type *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_string (be_string *node) override;
  int visit_structure (be_structure *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_union (be_union *node) override;
  int visit_valuebox (be_valuebox *node) override;
  int visit_valuetype (be_valuetype *node) override;

private:
  void emit_init_block ();
  void emit_error (const char *);
  void open_if_block ();
  void close_if_block ();

private:
  be_attribute *attr_;
};

#endif /* BE_VISITOR_ATTRIBUTE_CCM_INIT_H */

