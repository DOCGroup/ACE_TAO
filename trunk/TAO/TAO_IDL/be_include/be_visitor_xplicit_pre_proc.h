
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_xplicit_pre_proc.h
 *
 *  $Id$
 *
 *  This visitor creates the explicit home interface
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BE_VISITOR_XPLICIT_PRE_PROC_H
#define TAO_BE_VISITOR_XPLICIT_PRE_PROC_H

#include "be_visitor_ccm_pre_proc.h"

/**
 * @class be_visitor_xplicit_pre_proc
 *
 * @brief be_visitor_xplicit_pre_proc
 *
 * Adds home explicit implied IDL code to the AST.
 */
class be_visitor_xplicit_pre_proc : public be_visitor_ccm_pre_proc
{
public:
  be_visitor_xplicit_pre_proc (be_visitor_context *ctx);

  virtual ~be_visitor_xplicit_pre_proc (void);
  
  virtual int visit_home (be_home *node);

  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);
  virtual int visit_factory (be_factory *node);
  virtual int visit_finder (be_finder *node);
  
  // Member accessor
  be_interface *xplicit (void) const;
  
private:
  be_interface *xplicit_;
  be_home *node_;
};

#endif // TAO_BE_VISITOR_XPLICIT_PRE_PROC_H
