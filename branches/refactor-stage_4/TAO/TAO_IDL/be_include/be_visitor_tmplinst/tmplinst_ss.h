//=============================================================================
/**
*  @file   be_visitor_tmplinst_ss.h
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
#ifndef TAO_BE_VISITOR_TMPLINST_SS_H
#define TAO_BE_VISITOR_TMPLINST_SS_H

#include "tmplinst.h"

/**
 * @class be_visitor_tmplinst_ss
 *
 * @brief Generates explicit template instantiations for the skelton.
 */
class be_visitor_tmplinst_ss : public be_visitor_tmplinst
{
public:
  be_visitor_tmplinst_ss (be_visitor_context *ctx);

  virtual ~be_visitor_tmplinst_ss (void);

  virtual int visit_interface (be_interface *node);

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_operation (be_operation *node);

protected:
  virtual idl_bool this_mode_generated (be_decl *node) const;
  virtual void this_mode_generated (be_decl *node, 
                                    idl_bool val);
};


#endif // TAO_BE_VISITOR_TMPLINST_SS_H
