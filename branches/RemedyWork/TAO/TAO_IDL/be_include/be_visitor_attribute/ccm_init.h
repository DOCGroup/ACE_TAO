/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ccm_init.h
 *
 *  $Id$
 *
 *  Visitor generating a block of code to initialize
 *  each of a component or home's read/write attributes
 *
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
  ~be_visitor_attribute_ccm_init (void);

  virtual int visit_attribute (be_attribute *node);

  virtual int visit_array (be_array *node);
  virtual int visit_component (be_component *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_home (be_home *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);

private:
  void emit_init_block (void);
  void emit_error (const char *);
  void open_if_block (void);
  void close_if_block (void);

private:
  be_attribute *attr_;
};

#endif /* BE_VISITOR_ATTRIBUTE_CCM_INIT_H */

