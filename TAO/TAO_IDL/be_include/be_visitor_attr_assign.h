/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_attr_assign.h
 *
 *  Encapsulation of rules for component attribute assignment.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef BE_VISITOR_ATTR_ASSIGN_H
#define BE_VISITOR_ATTR_ASSIGN_H

#include "be_visitor_decl.h"

class TAO_OutStream;

/**
 * @class be_visitor_attr_assign
 *
 * @brief be_visitor_attr_assign
 *
 * Contains overloaded methods that generate assignment
 * statements in component attribute set operations.
 */
class be_visitor_attr_assign : public be_visitor_decl
{
public:
  be_visitor_attr_assign (be_visitor_context *ctx);
  ~be_visitor_attr_assign () override;

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

  void attr_name (const char *name);

private:
  const char *attr_name_;
  ACE_CString attr_name_string_;
};

#endif /* BE_VISITOR_ATTR_ASSIGN_H */

