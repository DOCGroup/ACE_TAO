//=============================================================================
/**
*  @file   be_visitor_arg_traits.h
*
*  This visitor generates template specializations for argument traits classes.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

#ifndef TAO_BE_VISITOR_ARG_TRAITS_H
#define TAO_BE_VISITOR_ARG_TRAITS_H

#include "be_visitor_scope.h"

/**
 * @class be_visitor_arg_traits
 *
 * @brief Generates specialized template argument traits declarations.
 */
class be_visitor_arg_traits : public be_visitor_scope
{
public:
  be_visitor_arg_traits (const char *S,
                         be_visitor_context *ctx);

  ~be_visitor_arg_traits () override;

  int visit_root (be_root *node) override;
  int visit_module (be_module *node) override;
  int visit_array (be_array *node) override;
  int visit_enum (be_enum *node) override;
  int visit_interface (be_interface *node) override;
  int visit_interface_fwd (be_interface_fwd *node) override;
  int visit_valuetype (be_valuetype *node) override;
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;
  int visit_eventtype (be_eventtype *node) override;
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;
  int visit_operation (be_operation *node) override;
  int visit_attribute (be_attribute *node) override;
  int visit_argument (be_argument *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_string (be_string *node) override;
  int visit_structure (be_structure *node) override;
  int visit_field (be_field *node) override;
  int visit_union (be_union *node) override;
  int visit_union_branch (be_union_branch *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_component (be_component *node) override;
  int visit_component_fwd (be_component_fwd *node) override;
  int visit_connector (be_connector *node) override;
  int visit_home (be_home *node) override;
  int visit_valuebox (be_valuebox *node) override;

private:

  bool generated (be_decl *node) const;
  void generated (be_decl *node,
                  bool val);

  const char *insert_policy ();

private:
  char *S_;
};


#endif // TAO_BE_VISITOR_ARG_TRAITS_H
