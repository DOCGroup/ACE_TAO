/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    set_from_extracted.h
 *
 *  $Id$
 *
 *  Encapsulation the variations in setting read/write
 *  attributes from values extracted from an Any
 *
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
  virtual ~be_visitor_attribute_set_from_extracted (void);

  virtual int visit_array (be_array *);
  virtual int visit_component (be_component *);
  virtual int visit_enum (be_enum *);
  virtual int visit_eventtype (be_eventtype *);
  virtual int visit_home (be_home *);
  virtual int visit_interface (be_interface *);
  virtual int visit_predefined_type (be_predefined_type *);
  virtual int visit_sequence (be_sequence *);
  virtual int visit_string (be_string *);
  virtual int visit_structure (be_structure *);
  virtual int visit_typedef (be_typedef *);
  virtual int visit_union (be_union *);
  virtual int visit_valuebox (be_valuebox *);
  virtual int visit_valuetype (be_valuetype *);
};

#endif /* BE_VISITOR_ATTRIBUTE_SET_FROM_EXTRACTED_H */

