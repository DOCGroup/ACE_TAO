//=============================================================================
/**
*  @file   be_visitor_arg_traits.h
*
*  $Id$
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
  be_visitor_arg_traits (char *S,
                         be_visitor_context *ctx);

  virtual ~be_visitor_arg_traits (void);

  virtual int visit_root (be_root *node);

  virtual int visit_module (be_module *node);

  virtual int visit_array (be_array *node);

  virtual int visit_enum (be_enum *node);

  virtual int visit_interface (be_interface *node);

  virtual int visit_interface_fwd (be_interface_fwd *node);

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  virtual int visit_eventtype (be_eventtype *node);

  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

  virtual int visit_sequence (be_sequence *node);

  virtual int visit_string (be_string *node);

  virtual int visit_structure (be_structure *node);

  virtual int visit_field (be_field *node);

  virtual int visit_union (be_union *node);

  virtual int visit_union_branch (be_union_branch *node);

  virtual int visit_typedef (be_typedef *node);

private:
  idl_bool generated (be_decl *node) const;
  void generated (be_decl *node,
                  idl_bool val);

private:
  char *S_;
};


#endif // TAO_BE_VISITOR_ARG_TRAITS_H
