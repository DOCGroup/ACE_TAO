//=============================================================================
/**
*  @file   be_visitor_tmplinst_cs.h
*
*  $Id$
*
*  This visitor generates explicit template instantiations, guarded so they
*  are seen only by compilers whose platforms require explicit template
*  instantiation.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================
#ifndef TAO_BE_VISITOR_TMPLINST_CS_H
#define TAO_BE_VISITOR_TMPLINST_CS_H

#include "be_visitor_scope.h"

/**
 * @class be_visitor_tmplinst
 *
 * @brief Generates explicit template instantiations.
 */
class be_visitor_tmplinst_cs : public be_visitor_scope
{
public:
  enum Mode
    {
      TMPL_CLASS,
      PRAGMA_INST
    };

  be_visitor_tmplinst_cs (be_visitor_context *ctx);

  virtual ~be_visitor_tmplinst_cs (void);

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

  virtual int visit_operation (be_operation *node);

  virtual int visit_sequence (be_sequence *node);

  virtual int visit_string (be_string *node);

  virtual int visit_structure (be_structure *node);

  virtual int visit_structure_fwd (be_structure_fwd *node);

  virtual int visit_field (be_field *node);

  virtual int visit_union (be_union *node);

  virtual int visit_union_fwd (be_union_fwd *node);

  virtual int visit_union_branch (be_union_branch *node);

  virtual int visit_typedef (be_typedef *node);

  void switch_mode (void);

private:
  idl_bool this_mode_generated (be_decl *node) const;
  void this_mode_generated (be_decl *node, 
                            idl_bool val);

private:
  Mode mode_;
  char * prefix_;
  char * suffix_;
  char * linebreak_;
};


#endif // TAO_BE_VISITOR_TMPLINST_CS_H
