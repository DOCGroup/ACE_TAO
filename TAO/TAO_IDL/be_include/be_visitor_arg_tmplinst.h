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

#include "be_visitor_tmplinst.h"

/**
 * @class be_visitor_arg_tmplinst
 *
 * @brief Generates explicit template instantiations for arg helper classes.
 */
class be_visitor_arg_tmplinst : public be_visitor_tmplinst
{
public:
  be_visitor_arg_tmplinst (be_visitor_tmplinst *rhs);

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

  virtual int visit_field (be_field *node);

  virtual int visit_union (be_union *node);

  virtual int visit_union_branch (be_union_branch *node);

  virtual int visit_typedef (be_typedef *node);

protected:
  virtual void cleanup (void);
};


#endif // TAO_BE_VISITOR_ARG_TMPLINST_H
