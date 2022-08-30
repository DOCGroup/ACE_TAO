/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_any_extracted_type_decl.h
 *
 *  Encapsulation of rules for declaring variables
 *  for Any extraction
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef BE_VISITOR_ANY_EXTRACTED_TYPE_DECL_H
#define BE_VISITOR_ANY_EXTRACTED_TYPE_DECL_H

#include "be_visitor_decl.h"

class TAO_OutStream;

/**
 * @class be_visitor_any_extracted_type_decl
 *
 * @brief be_visitor_any_extracted_type_decl
 *
 * Contains overloaded methods that generate declarations of
 * CORBA typenames for Any extraction
 */
class be_visitor_any_extracted_type_decl : public be_visitor_decl
{
public:
  be_visitor_any_extracted_type_decl (be_visitor_context *ctx);
  ~be_visitor_any_extracted_type_decl () override;

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

protected:
  const char * var_name_;
  const char * tmp_name_;
};

#endif /* BE_VISITOR_ANY_EXTRACTED_TYPE_DECL_H */

