//=============================================================================
/**
*  @file   be_visitor_arg_tmplinst.h
*
*  $Id$
*
*  This visitor generates explicit template instantiations, guarded so they
*  are seen only by compilers whose platforms require explicit template
*  instantiation, for the individual argument template helper classes.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================
#ifndef TAO_BE_VISITOR_ARG_TMPLINST_H
#define TAO_BE_VISITOR_ARG_TMPLINST_H

#include "be_visitor_tmplinst_cs.h"
#include "ast_argument.h"

/**
 * @class be_visitor_arg_tmplinst
 *
 * @brief Generates explicit template instantiations for arg helper classes.
 */
class be_visitor_arg_tmplinst : public be_visitor_decl
{
public:
  be_visitor_arg_tmplinst (be_visitor_context *ctx,
                           be_visitor_tmplinst_cs::Mode mode,
                           char * prefix,
                           char * suffix,
                           char * linebreak,
                           const char * S);

  virtual ~be_visitor_arg_tmplinst (void);

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

  virtual int visit_structure_fwd (be_structure_fwd *node);

  virtual int visit_field (be_field *node);

  virtual int visit_union (be_union *node);

  virtual int visit_union_fwd (be_union_fwd *node);

  virtual int visit_union_branch (be_union_branch *node);

  virtual int visit_typedef (be_typedef *node);

  // Accdessors for the member.
  void direction (void);
  void direction (AST_Argument::Direction dir);

private:
  // Set/get the appropriate flag on the node we are traversing,
  // to make sure we don't generate something twice.
  idl_bool this_mode_and_dir_generated (be_decl *node) const;
  void this_mode_and_dir_generated (be_decl *node, 
                                    idl_bool val);

  // Generate the direction prefix to the arg class name.
  void gen_direction (TAO_OutStream *os);

private:
  enum Direction
    {
      _tao_IN,
      _tao_INOUT,
      _tao_OUT,
      _tao_RET
    };

  // Generating 'template class' or '#pragma instantiate'.
  be_visitor_tmplinst_cs::Mode mode_;

  // Contains the prefix string itself.
  char * prefix_;

  // ';' or empty string
  char * suffix_;

  // backslash for '#pragma instantiate' mode, empty otherwise.
  char * linebreak_;

  // Value of the above enum we are holding.
  Direction dir_;

  // Contains 'S' to insert in skel side arg class name, or empty.
  const char * S_;
};


#endif // TAO_BE_VISITOR_ARG_TMPLINST_H
