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

#include "tmplinst.h"

/**
 * @class be_visitor_tmplinst_cs
 *
 * @brief Generates explicit template instantiations for the stub.
 */
class be_visitor_tmplinst_cs : public be_visitor_tmplinst
{
public:
  be_visitor_tmplinst_cs (be_visitor_context *ctx);

  virtual ~be_visitor_tmplinst_cs (void);

  virtual int visit_array (be_array *node);

  virtual int visit_enum (be_enum *node);

  virtual int visit_interface (be_interface *node);

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_operation (be_operation *node);

  virtual int visit_sequence (be_sequence *node);

  virtual int visit_string (be_string *node);

  virtual int visit_structure (be_structure *node);

  virtual int visit_union (be_union *node);

protected:
  virtual idl_bool this_mode_generated (be_decl *node) const;
  virtual void this_mode_generated (be_decl *node, 
                                    idl_bool val);

  // Generate explicit template instantiations for our _var
  // and _out classes.
  int gen_varout_tmplinst (be_sequence *node,
                           be_type *bt);

  // Generate explicit instantiation for our element type, if
  // it is a managed type.
  void gen_managed_type_tmplinst (be_sequence *node,
                                  be_type *bt);

  // Generate explicit instantiation for the base class type.
  int gen_base_class_tmplinst (be_sequence *node);

  void gen_common_tmplinst (be_type *node,
                            TAO_OutStream *os);
  // Generate explicit template instantiations for structs and unions.
};


#endif // TAO_BE_VISITOR_TMPLINST_CS_H
